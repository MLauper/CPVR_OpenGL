///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-04_D.01_BasicRendering - Toonify, (Ver 2.1)                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////



// system includes ////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
using namespace std;


// OpenGL helper includes /////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#ifdef _MSC_VER
#pragma warning( disable: 4312 ) // ignore visual studio warnings for FLTK 64-bit type casts
#endif
#include <FL/glut.H>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// application helper includes ////////////////////////////////////////////////////////////////////
#include "../../_COMMON/inc/TrackBall.h"
#include "../../_COMMON/inc/UtilGLSL.h"
#include "../inc/vboteapot.h"


// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MV_MAT4_LOCATION = 0;
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

VBOTeapot* TheTeapot = 0;



void drawModel(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	TheTeapot->draw();
}



void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// clear window background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get trackball transformation matrix
	glm::mat4 model(1.0f);
	glm::mat4 view = TrackBall::getTransformation();
	model = model * view;

	// move teapot into orgin and rotate 90 degree around x-axis (post multiply order)
	model = glm::rotate(model, glm::radians<float>(-90.0), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));

	// set model view transformation matrix
	glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));

	// draw teapot with offset around origin
	drawModel();

	glutSwapBuffers();
	UtilGLSL::checkOpenGLErrorCode();
}



void initModel(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glm::mat4 lidTransform(1.0);
	TheTeapot = new VBOTeapot(16, lidTransform);
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// set background color
	glClearColor(1.0f, 1.0f, 1.0f, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// get modelview matrix location
	MV_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");
}



void glutResizeCB(int w, int h)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// adust perspective projection for new viewport
	GLfloat aspect = (GLfloat)w / (GLfloat)h;

	glm::mat4 projection = glm::perspective(20.0f, aspect, 1.0f, 20.0f);
	glm::mat4 camera = glm::lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	projection = projection * camera;
	GLint location = glGetUniformLocation(PROGRAM_ID, "matProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
}



void glutMenuCB(int item)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	int argc = 0; char* argv[10];

	switch (item)
	{
		case 0:
		{
			exit(0);
			break;
		}
		case 1:
		{
			// delete old shader program and setup toonify1 shader files
			if (PROGRAM_ID)	glDeleteProgram(PROGRAM_ID);

			argc = 4;
			argv[0] = "";
			argv[1] = "../../glsl/toonify.vert";
			argv[2] = "../../glsl/toonify.frag";
			argv[3] = "../../glsl/toonify1.frag";
			PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);

			// reset the projection and camera matrix uniform of new shader program
			glutResizeCB(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			break;
		}
		case 2:
		{
			// delete old shader program and setup toonify2 shader files
			if (PROGRAM_ID) glDeleteProgram(PROGRAM_ID);

			argc = 4;
			argv[0] = "";
			argv[1] = "../../glsl/toonify.vert";
			argv[2] = "../../glsl/toonify.frag";
			argv[3] = "../../glsl/toonify2.frag";
			PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);

			// reset the projection and camera matrix uniform of new shader program
			glutResizeCB(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			break;
		}
		default:
		{
			// reset settings
		}
	}
	glutPostRedisplay();
}



void initMenu()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// create menu
	glutCreateMenu(glutMenuCB);
	glutAddMenuEntry("Use Toonify1 Shader", 1);
	glutAddMenuEntry("Use Toonify2 Shader", 2);
	glutAddMenuEntry("Exit", 0);

	// attach menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}



void glutKeyboardCB(unsigned char key, int x, int y)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	switch (key)
	{
		case 27:
		{
			exit(0);
			break;
		}
	}
	glutPostRedisplay();
}



int main(int argc, char *argv[])
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | FL_OPENGL3);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 640);
	glutCreateWindow("Basic Rendering");
    
	// register extension wrapper library (GLEW)
	glewExperimental = GL_TRUE;
	// necessary to force GLEW to use a modern OpenGL method for function availability check

	if(glewInit() != GLEW_OK)
	{
		std::cout << "ERROR: GLEW not initialized: " << glewInit() << endl;
		return -1;
	}

	// show version of OpenGL and GLSL
	UtilGLSL::showOpenGLVersion();
	UtilGLSL::showGLSLVersion();
	UtilGLSL::initOpenGLDebugCallback();

	// check for shader 4.x support
	if (UtilGLSL::checkOpenGLVersion() < 4.0)
	{
		cout << "Program requires OpenGL Version >= 4.x (Supported: ";
		cout << UtilGLSL::checkOpenGLVersion() << ") - exiting!" << endl;
		cout << "Press ENTER to continue... " << endl;
		cin.ignore();
		exit(1);
	}

	// register GLUT/FLTK callbacks
	glutDisplayFunc(glutDisplayCB);
	glutKeyboardFunc(glutKeyboardCB);
	glutReshapeFunc(glutResizeCB);

	glutMouseFunc(TrackBall::glutMouseButtonCB);
	glutMotionFunc(TrackBall::glutMouseMotionCB);
	glutSpecialFunc(TrackBall::glutSpecialFuncCB);

	// check for command line argument supplied shaders
	if (argc > 1)
	{
		PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
	}
	else
	{
		argc = 4;
		argv[0] = "";
		argv[1] = "../../glsl/toonify.vert";
		argv[2] = "../../glsl/toonify.frag";
		argv[3] = "../../glsl/toonify1.frag";
		PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
	}

	// init application
	initRendering();
	initModel();
	initMenu();

	// entering GLUT/FLTK main rendering loop
	glutMainLoop();
	return 0;  // only for compatibility purposes
}
