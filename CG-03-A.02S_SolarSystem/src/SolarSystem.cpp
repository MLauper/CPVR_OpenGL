///////////////////////////////////////////////////////////////////////////////////////////////////
// Assignment: CG-03-A.02_SolarSystem Solution (Ver 2.1)                                         //
///////////////////////////////////////////////////////////////////////////////////////////////////



// system includes ////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <stack>
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
#include <glm/gtc/constants.hpp>


// application helper includes ////////////////////////////////////////////////////////////////////
#include "../../_COMMON/inc/TrackBall.h"
#include "../../_COMMON/inc/UtilGLSL.h"
#include "../inc/Sphere.h"


// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MV_MAT4_LOCATION = 0;
GLint COLOR_VEC3_LOCATION = 0;
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int MENU_ENTRY = 0;
int MENU_VALUE = 0;
string MENU_ENTRY_STR[3];


Sphere MySphere;
float SUN_ROTATION = 0.0f;
float P1_ANGLE = 0.0f;
float P1_ROTATION = 0.0f;
float P2_ANGLE = 0.0f;
float P2_ROTATION = 0.0f;
float M1_ANGLE = 0.0f;
float M2_ANGLE = 0.0f;

bool ANIMATION_RUNNING = false;
int SPEED = 20;



void glutTimerCB(int timer_id)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	SUN_ROTATION += 0.01f;
	P1_ANGLE += 0.02f;
	P1_ROTATION -= 0.05f;
	P2_ANGLE -= 0.03f;
	P2_ROTATION += 0.1f;
	M1_ANGLE -= 0.01f;
	M2_ANGLE -= 0.03f;

	// reset timer if the animation is running
	if (ANIMATION_RUNNING) glutTimerFunc(SPEED, glutTimerCB, 0);

	glutPostRedisplay();
}


void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// clear window background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get trackball transformation matrix
	stack<glm::mat4> model_view_stack;
	glm::mat4 model(1.0f);
	glm::mat4 view = TrackBall::getTransformation();
	model = model * view;


	// draw sun ///////////////////////////////////////////////////////////////
	model_view_stack.push(model);
		model = glm::scale(model, glm::vec3(-2.0f, -2.0f, -2.0f));
		model = glm::rotate(model, SUN_ROTATION, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(COLOR_VEC3_LOCATION, 1.0, 1.0, 0.0); // yellow
		MySphere.draw();
		model = model_view_stack.top();
	//model_view_stack.pop();


	// draw planet 1 /////////////////////////////////////////////////////////////
	//model_view_stack.push(model);
		model = glm::rotate(model, P1_ANGLE, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		model = glm::rotate(model, P1_ROTATION, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(COLOR_VEC3_LOCATION, 0.0f, 0.0f, 1.0f); // blue
		glutSolidSphere(0.5, 20, 10);  // strictly this GLUT call is not OpenGL core compatible

		// draw planet 1 moon 1
		model = glm::translate(model, glm::vec3(1.1f, 0.0f, 0.0f));
		model = glm::rotate(model, P1_ROTATION, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(-0.1f, -0.1f, -0.1f));
		glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));
		glutWireTeapot(0.2);  // strictly this GLUT call is not OpenGL core compatible
		model = model_view_stack.top();
	//model_view_stack.pop();


	// draw planet 2 //////////////////////////////////////////////////////////
	//model_view_stack.push(model);
		model = glm::rotate(model, P2_ANGLE, glm::vec3(0.0, 0.0, 1.0));
		model = glm::translate(model, glm::vec3(7.0, 0.0, 0.0));
		model = glm::rotate(model, P2_ROTATION, glm::vec3(0.0, 0.0, 1.0));
	model_view_stack.push(model);
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(COLOR_VEC3_LOCATION, 0.0f, 1.0f, 0.0f); // green
		glutSolidDodecahedron();  // strictly this GLUT call is not OpenGL core compatible
		model = model_view_stack.top();
	//model_view_stack.pop();

	// draw planet 2 moon 1
	//model_view_stack.push(model);
		model = glm::rotate(model, M2_ANGLE, glm::vec3(0.0, 0.0, 1.0));
		model = glm::translate(model, glm::vec3(1.5F, 0.0, 0.0));
		model = glm::rotate(model, P2_ROTATION, glm::vec3(0.0, 0.0, 1.0));
		glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));
		glutSolidSphere(0.25, 10, 10);  // strictly this GLUT call is not OpenGL core compatible
		model = model_view_stack.top();
	model_view_stack.pop();

	// draw planet 2 moon 2
		model = glm::rotate(model, M1_ANGLE, glm::vec3(0.0, 0.0, 1.0));
		model = glm::translate(model, glm::vec3(2.5, 0.0, 0.0));
		model = glm::rotate(model, P1_ROTATION, glm::vec3(0.0, 0.0, 1.0));
		model = glm::scale(model, glm::vec3(1.5, 0.75, 0.75));
		glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));
		glutSolidSphere(0.5, 10, 10);  // strictly this GLUT call is not OpenGL core compatible
		model = model_view_stack.top();
	//model_view_stack.pop();


	// draw planet 3 //////////////////////////////////////////////////////////
	//model_view_stack.push(model);
		model = glm::rotate(model, P1_ANGLE, glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, glm::vec3(5.5, 0.0, 0.0));
		model = glm::rotate(model, -P1_ANGLE, glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model, P1_ROTATION, glm::vec3(0.0, 0.0, 1.0));
		glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(COLOR_VEC3_LOCATION, 0.0f, 1.0f, 1.0f); // cyan
		glutSolidSphere(0.5, 20, 10);  // strictly this GLUT call is not OpenGL core compatible

		// draw planet 3 moon 1
		model = glm::rotate(model, M2_ANGLE, glm::vec3(0.0, 0.0, 1.0));
		model = glm::translate(model, glm::vec3(1.5, 0.0, 0.0));
		model = glm::rotate(model, P2_ROTATION, glm::vec3(0.0, 0.0, 1.0));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));
		glutSolidOctahedron();  // strictly this GLUT call is not OpenGL core compatible
		model = model_view_stack.top();
	model_view_stack.pop();

	glutSwapBuffers();
	UtilGLSL::checkOpenGLErrorCode();
}



void initModel(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// get position vertex attribute location
	// (requires that the shader program has been compiled already!)
	GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
	MySphere.init(vecPosition);
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// set background color
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// get and setup orthographic projection matrix
	glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
	GLint location = glGetUniformLocation(PROGRAM_ID, "matProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));

	// get modelview matrix location
	MV_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");
	COLOR_VEC3_LOCATION = glGetUniformLocation(PROGRAM_ID, "vecColor");
}



void initMenuChange(int entry, char *name, int value)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	MENU_ENTRY = entry;
	MENU_VALUE = value;
	if (entry > 0) MENU_ENTRY_STR[entry].assign(name);
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
		case 1:
		{
			initMenuChange(1, "Stop Animation", 2);
			ANIMATION_RUNNING = true;
			glutTimerFunc(SPEED, glutTimerCB, 0);
			break;
		}
		case 2:
		{
			initMenuChange(1, "Start Animation", 1);
			ANIMATION_RUNNING = false;
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
	// work around for FLTK bug in glutChangeToMenuEntry() 
	MENU_ENTRY_STR[0] = ""; // dummy entry since GLUT menus start at index 1
	MENU_ENTRY_STR[1] = "Start Animation";

	// create menu
	glutCreateMenu(glutMenuCB);
	glutAddMenuEntry(const_cast<char*>(MENU_ENTRY_STR[1].c_str()), 1);
	glutAddMenuEntry("Exit", 0);

	// attach menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}



void glutUpdateMenuCB(int status, int x, int y)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	if (status == GLUT_MENU_NOT_IN_USE)
	{
		// menu not in use, time to make changes
		if (MENU_ENTRY < 0)
		{
			// reset menu to default
			cout << "Resetting Menu..." << endl;
			initMenu();
		}
		else
		{
			glutChangeToMenuEntry(MENU_ENTRY, const_cast<char*>(MENU_ENTRY_STR[MENU_ENTRY].c_str()), MENU_VALUE);
		}
	}
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
		case ' ':
		{
			if (ANIMATION_RUNNING)
			{
				glutMenuCB(2);
			}
			else
			{
				glutMenuCB(1);
			}
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
	glutCreateWindow("Solar System");

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
		exit(1);
	}

	// register GLUT/FLTK callbacks
	glutDisplayFunc(glutDisplayCB);
	glutKeyboardFunc(glutKeyboardCB);
	glutMenuStatusFunc(glutUpdateMenuCB);

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
		argv[1] = "../../glsl/solarsystem.vert";
		argv[2] = "../../glsl/solarsystem.frag";
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
