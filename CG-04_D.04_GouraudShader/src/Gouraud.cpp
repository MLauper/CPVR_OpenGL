///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-04_D.03_GouraudShader (Ver 2.1)                                                     //
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
	glm::mat4 mouse = TrackBall::getTransformation();


	// move teapot into orgin and rotate 270 degree around x-axis (post multiply order)
	glm::mat4 teapot_matrix(1.0);
	teapot_matrix = glm::rotate(teapot_matrix, glm::radians<float>(270.0), glm::vec3(1.0, 0.0, 0.0));
	teapot_matrix = glm::translate(teapot_matrix, glm::vec3(0.0, 0.0, -1.5));

	glm::mat4 model(1.0f);
	model = model * mouse;
	model = model * teapot_matrix;

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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// define fixed light source position and properties
	glm::vec4 position(0.0f, 0.0f, 20.0f, 1.0f);
	glm::vec4  ambient(0.8f, 0.8f,  0.8f, 1.0f);
	glm::vec4  diffuse(0.8f, 0.8f,  0.8f, 1.0f);
	glm::vec4 specular(1.0f, 1.0f,  1.0f, 1.0f);

	// define material properties
	glm::vec4  mat_ambient(0.1f, 0.1f, 0.1f, 1.0f);
	glm::vec4  mat_diffuse(0.1f, 0.1f, 0.9f, 1.0f);
	glm::vec4 mat_specular(1.0f, 1.0f, 1.0f, 1.0f);
	GLfloat  mat_shininess = 64.0f;

	// setup Uniform Buffer Objects (UBO)
	GLuint ubo[2];
	glGenBuffers(2, &ubo[0]);

	glBindBuffer(GL_UNIFORM_BUFFER, ubo[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 4, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(position));
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(ambient));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(diffuse));
	glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(specular));

	glBindBuffer(GL_UNIFORM_BUFFER, ubo[1]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 3 + sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(mat_ambient));
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(mat_diffuse));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(mat_specular));
	glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(GLfloat), &mat_shininess);


	// bind uniform buffer objects to uniform blocks using the shader provided binding point
	GLuint ubo_index = 0; 
	GLint ubo_binding = 0;

	ubo_index = glGetUniformBlockIndex(PROGRAM_ID, "LightProperties");
	glGetActiveUniformBlockiv(PROGRAM_ID, ubo_index, GL_UNIFORM_BLOCK_BINDING, &ubo_binding);
	glBindBufferBase(GL_UNIFORM_BUFFER, ubo_binding, ubo[0]);

	ubo_index = glGetUniformBlockIndex(PROGRAM_ID, "MaterialProperties");
	glGetActiveUniformBlockiv(PROGRAM_ID, ubo_index, GL_UNIFORM_BLOCK_BINDING, &ubo_binding);
	glBindBufferBase(GL_UNIFORM_BUFFER, ubo_binding, ubo[1]);

	// get modelview matrix location
	MV_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");
}



void glutResizeCB(int w, int h)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// adust perspective projection for new viewport
	GLfloat aspect = (GLfloat)w / (GLfloat)h;

	//glm::mat4 projection = glm::perspective(20.0f, aspect, 1.0f, 20.0f);
	glm::mat4 projection = glm::perspective(glm::radians<float>(80.0f), aspect, 1.0f, 20.0f);
	glm::mat4 camera = glm::lookAt(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	projection = projection * camera;
	GLint location = glGetUniformLocation(PROGRAM_ID, "matProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
}



void glutMenuCB(int item)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	switch (item)
	{
		case 0:
		{
			exit(0);
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
	glutCreateWindow("Gouraud Shader");

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
		argc = 3;
		argv[0] = "";
		argv[1] = "../../glsl/gouraud.vert";
		argv[2] = "../../glsl/gouraud.frag";
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
