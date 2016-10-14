///////////////////////////////////////////////////////////////////////////////////////////////////
// Assignment: CG-03-A.01_3DHouse (Ver 2.1)                                                      //
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


// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MV_MAT4_LOCATION = 0;
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

int MENU_ENTRY = 0;
int MENU_VALUE = 0;
string MENU_NAME;

GLenum POLYGON_MODE = GL_FRONT;
GLenum POLYGON_DRAW = GL_FILL;

const float WIDTH = 5.0f;		// ground = width * 2
const float HEIGHT = 3.0f;		// height of roof



void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// clear window background
	glClear(GL_COLOR_BUFFER_BIT);

	// get trackball transformation matrix
	glm::mat4 model(1.0f);
	glm::mat4 view = TrackBall::getTransformation();
	model = model * view;

	// set model view transformation matrix
	glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));

	// definition of ground face indices (for GL_TRIANGLES --> 6)
	static GLushort ground_indices[] =
	{
		0, 3, 2, 
		1, 0, 2
	};

	// dereferencing the vertices and draw the geometry using glDrawElements
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, &ground_indices[0]);

	glutSwapBuffers();
	UtilGLSL::checkOpenGLErrorCode();
}



void initModel(float witdth, float heigth)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// define ground vertices
	GLfloat ground_vertices[] =
	{
		-witdth, -witdth, 0.0f, 1.0f,  // v0
		 witdth, -witdth, 0.0f, 1.0f,  // v1
		 witdth,  witdth, 0.0f, 1.0f,  // v2
		-witdth,  witdth, 0.0f, 1.0f   // v3
	};

	// definition of the colors, each vertex has his own color definition (RGB)
	GLfloat ground_colors[] =
	{
		0.0f, 0.4f, 0.0f,
		0.0f, 0.4f, 0.0f,
		0.0f, 0.4f, 0.0f,
		0.0f, 0.4f, 0.0f
	};


	// setup and bind Vertex Array Object for plane
	GLuint vao;
	glGenVertexArrays(1, &vao);	
	glBindVertexArray(vao);

	// setup Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices) + sizeof(ground_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ground_vertices), ground_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(ground_vertices), sizeof(ground_colors), ground_colors);

	// get vertex position attribute location and setup vertex attribute pointer
	// (requires that the shader program has been compiled already!)
	GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
	glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vecPosition);
	
	// get vertex color attribute location and setup vertex attribute pointer
	// (requires that the shader program has been compiled already!)
	GLuint vecColor = glGetAttribLocation(PROGRAM_ID, "vecColor");
	glVertexAttribPointer(vecColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(ground_vertices)));
	glEnableVertexAttribArray(vecColor);
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// set background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPolygonMode(POLYGON_MODE, GL_FILL);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	// get and setup orthographic projection matrix
	glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
	GLint location = glGetUniformLocation(PROGRAM_ID, "matProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));

	// get modelview matrix location
	MV_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");
}


void initMenuChange(int entry, char* name, int value)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	MENU_ENTRY = entry;
	MENU_VALUE = value;
	MENU_NAME.assign(name);
}



void glutMenuCB1(int item)
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
			glPolygonMode(POLYGON_MODE, GL_LINE);
			POLYGON_DRAW = GL_LINE;
			initMenuChange(1, "Disable Wireframe", 2);
			break;
		}
		case 2:
		{
			glPolygonMode(POLYGON_MODE, GL_FILL);
			POLYGON_DRAW = GL_FILL;
			initMenuChange(1, "Enable Wireframe", 1);
			break;
		}
		case 3:
		{
			initMenuChange(2, "Disable Culling", 4);
			break;
		}
		case 4:
		{
			initMenuChange(2, "Enable Culling", 3);
			break;
		}
		case 5:
		{
			initMenuChange(3, "Disable Depth Buffer", 6);
			glutSetWindowTitle("3D House (Depth Buffering Enabled)");
			break;
		}
		case 6:
		{
			initMenuChange(3, "Enable Depth Buffer", 5);
			glutSetWindowTitle("3D House (Depth Buffering Disabled)");
			break;
		}
		default:
		{
			// reset settings
			glPolygonMode(POLYGON_MODE, GL_FILL);
			POLYGON_DRAW = GL_FILL;
			glutSetWindowTitle("3D House");
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);

			initMenuChange(-1, "Reset Menu", -1);
		}
	}
	glutPostRedisplay();
}



void glutMenuCB2(int item)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
    POLYGON_MODE = item;
	glPolygonMode(POLYGON_MODE, POLYGON_DRAW);
    glutPostRedisplay();
}



void initMenu()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	int sub_menu;

	/* create sub-menu */
	sub_menu = glutCreateMenu(glutMenuCB2);
	glutAddMenuEntry("GL_FRONT", GL_FRONT);
	glutAddMenuEntry("GL_BACK", GL_BACK);
	glutAddMenuEntry("GL_FRONT_AND_BACK", GL_FRONT_AND_BACK);


	// create menu
	glutCreateMenu(glutMenuCB1);
	glutAddMenuEntry("Enable Wireframe", 1);
	glutAddMenuEntry("Enable Culling", 3);
	glutAddMenuEntry("Enable Depth Buffer", 5);
	glutAddSubMenu("Set Polygon Face", sub_menu);
	glutAddMenuEntry("Reset", 9);
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
			MENU_NAME.append(1, 0); // guarantee NUL termination
			MENU_NAME.resize(1024);
			glutChangeToMenuEntry(MENU_ENTRY, &MENU_NAME[0], MENU_VALUE);
			MENU_NAME.resize(strlen(&MENU_NAME[0]));
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
	}
}



int main(int argc, char *argv[])
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | FL_OPENGL3);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 640);
	glutCreateWindow("3D House");

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
		argv[1] = "../../glsl/house.vert";
		argv[2] = "../../glsl/house.frag";
		PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
	}

	// init application 
	initRendering();
	initModel(WIDTH, HEIGHT);
 	initMenu();

	glutMainLoop();
	return 0;  // only for compatibility purposes
}
