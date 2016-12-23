///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-06_D.02_QuadTessellation (Ver 2.1)                                                   //
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
GLint INNER_TESSLEVEL_ID = 0;
GLint OUTER_TESSLEVEL_ID = 0;
GLuint VAO_HANDLE = 0;

int INNER_TESSLEVEL = 1;
int OUTER_TESSLEVEL = 1;


void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// clear window background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get trackball transformation matrix
	glm::mat4 mouse = TrackBall::getTransformation();

	glm::mat4 model(1.0f);
	model = model * mouse;

	// set model view transformation matrix
	glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));

	// draw model
	glBindVertexArray(VAO_HANDLE);
	glDrawArrays(GL_PATCHES, 0, 4);

	glutSwapBuffers();
	UtilGLSL::checkOpenGLErrorCode();
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// set background color
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	// setup 2D vertices for patch primitive
	float vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };

	// setup Vertex Buffer Object (VBO)
	GLuint vboHandle;
	glGenBuffers(1, &vboHandle);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_STATIC_DRAW);

	// setup Vertex Array Object (VAO)
	glGenVertexArrays(1, &VAO_HANDLE);
	glBindVertexArray(VAO_HANDLE);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// set the number of vertices per patch.  IMPORTANT!!
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	GLint maxVerts;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxVerts);
	cout << "Max patch vertices supported:" << maxVerts << endl;


	MV_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");
	INNER_TESSLEVEL_ID = glGetUniformLocation(PROGRAM_ID, "uiInnerTessLevel");
	OUTER_TESSLEVEL_ID = glGetUniformLocation(PROGRAM_ID, "uiOuterTessLevel");

	glUniform1i(INNER_TESSLEVEL_ID, INNER_TESSLEVEL);
	glUniform1i(OUTER_TESSLEVEL_ID, OUTER_TESSLEVEL);
}



void glutResizeCB(int w, int h)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// adust perspective projection for new viewport
	GLfloat aspect = (GLfloat)w / (GLfloat)h;

	float c = 3.5f;
	glm::mat4 projection = glm::ortho(-0.4f * c, 0.4f * c, -0.4f *c, 0.4f*c, 0.1f, 100.0f);
	glm::mat4 camera = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	projection = projection * camera;
	GLint location = glGetUniformLocation(PROGRAM_ID, "matProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));

	float w2 = w / 2.0f;
	float h2 = h / 2.0f;
	glm::mat4 viewport = glm::mat4(
		glm::vec4(w2, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, h2, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
		glm::vec4(w2 + 0, h2 + 0, 0.0f, 1.0f));

	location = glGetUniformLocation(PROGRAM_ID, "matViewport");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewport));
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
			INNER_TESSLEVEL++;
			glUniform1i(INNER_TESSLEVEL_ID, INNER_TESSLEVEL);
			break;
		}
		case 2:
		{
			INNER_TESSLEVEL--;
			if (INNER_TESSLEVEL == 0) INNER_TESSLEVEL = 1;
			glUniform1i(INNER_TESSLEVEL_ID, INNER_TESSLEVEL);
			break;
		}
		case 3:
		{
			OUTER_TESSLEVEL++;
			glUniform1i(OUTER_TESSLEVEL_ID, OUTER_TESSLEVEL);
			break;
		}
		case 4:
		{
			OUTER_TESSLEVEL--;
			if (OUTER_TESSLEVEL == 0) OUTER_TESSLEVEL = 1;
			glUniform1i(OUTER_TESSLEVEL_ID, OUTER_TESSLEVEL);
			break;
		}
		case 5:
		{
			INNER_TESSLEVEL = 1;
			OUTER_TESSLEVEL = 1;
			glUniform1i(INNER_TESSLEVEL_ID, INNER_TESSLEVEL);
			glUniform1i(OUTER_TESSLEVEL_ID, OUTER_TESSLEVEL);
			break;
		}
		default:
		{
			// reset settings
		}
	}

	cout << "Tess Control Parameters: Inner Level=" << INNER_TESSLEVEL << ", Outer Level=" << OUTER_TESSLEVEL << endl;

	glutPostRedisplay();
}



void initMenu()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// create menu
	glutCreateMenu(glutMenuCB);
	glutAddMenuEntry("Exit", 0);
	glutAddMenuEntry("Increase Inner Level [1]", 1);
	glutAddMenuEntry("Decrease Inner Level [2]", 2);
	glutAddMenuEntry("Increase Outer Level [3]", 3);
	glutAddMenuEntry("Decrease Outer Level [4]", 4);
	glutAddMenuEntry("Reset Tess Levels    [5]", 5);

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
		case 49:
		{
			glutMenuCB(1);
			break;
		}
		case 50:
		{
			glutMenuCB(2);
			break;
		}
		case 51:
		{
			glutMenuCB(3);
			break;
		}
		case 52:
		{
			glutMenuCB(4);
			break;
		}
		case 53:
		{
			glutMenuCB(5);
			break;
		}
		default:
		{
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
	glutCreateWindow("Quad Tessellation Shader");

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
		argc = 6;
		argv[0] = "";
		argv[1] = "../../glsl/QuadTessellation.vert";
		argv[2] = "../../glsl/QuadTessellation.frag";
		argv[3] = "../../glsl/QuadTessellation.geom";
		argv[4] = "../../glsl/QuadTessellation.tecs";
		argv[5] = "../../glsl/QuadTessellation.tess";
		PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
	}

	// init application
	initRendering();
	initMenu();

	// entering GLUT/FLTK main rendering loop
	glutMainLoop();
	return 0;  // only for compatibility purposes
}

