///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-03-D.01D_BasicModeling (Ver 2.1)                                                     //
///////////////////////////////////////////////////////////////////////////////////////////////////



// system includes ////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <iomanip>
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
string MENU_ENTRY_STR[4];

const float WIDTH = 3.0f;		// width of cube
GLuint INDICES_COUNT = 0; 

GLfloat TRANSPARENCY = 1.0;



void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// clear window background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get trackball transformation matrix
	glm::mat4 model(1.0f);
	glm::mat4 view = TrackBall::getTransformation();
	model = model * view;

	// set model view transformation matrix
	glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));

	// dereferencing the vertices and draw the geometry using glDrawElements
	glDrawElements(GL_TRIANGLES, INDICES_COUNT, GL_UNSIGNED_SHORT, 0);

	glutSwapBuffers();
	UtilGLSL::checkOpenGLErrorCode();
}



void initModel(float witdth)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// definition of the cube vertices (X, Y, Z)
	GLfloat vertices[] =
	{
		-witdth, -witdth,  witdth, 1.0f,  // v0
		 witdth, -witdth,  witdth, 1.0f,  // v1
		 witdth,  witdth,  witdth, 1.0f,  // v2
		-witdth,  witdth,  witdth, 1.0f,  // v3
		-witdth, -witdth, -witdth, 1.0f,  // v4
		-witdth,  witdth, -witdth, 1.0f,  // v5
		 witdth,  witdth, -witdth, 1.0f,  // v6
		 witdth, -witdth, -witdth, 1.0f   // v7
	};

	// definition of the colors, each vertex has his own color definition (RGB)
	GLfloat colors[] =
	{
		0.0, 0.0, 1.0, 1.0, // BLUE
		1.0, 0.0, 1.0, 1.0, // MAGENTA (B+R)
		1.0, 1.0, 1.0, 1.0, // WHITE
		0.0, 1.0, 1.0, 1.0, // CYAN (B+G)
		0.0, 0.0, 0.0, 1.0, // BLACK
		0.0, 1.0, 0.0, 1.0, // GREEN
		1.0, 1.0, 0.0, 1.0, // YELLOW (R+G)
		1.0, 0.0, 0.0, 1.0  // RED
	};

	// definition of cube face indices (for GL_TRIANGLE_STRIP --> 15)
	static GLushort indices[] =
	{
		// side faces
		3, 0, 2,
		2, 0, 1,
		1, 7, 2,
		2, 7, 6,
		6, 7, 4,
		4, 5, 6,
		4, 0, 3,
		3, 5, 4,
		// top faces
		3, 2, 5,
		5, 2, 6,
		// bottom faces
		0, 4, 1,
		1, 4, 7
	};
	INDICES_COUNT = sizeof(indices) / sizeof(indices[0]);

	// setup Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);	
	glBindVertexArray(vao);

	// setup Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

	// setup local Element Buffer Object (Indices)
	GLuint ebo;
	glGenBuffers(1, &ebo);

	// setup Indices Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

	// get position vertex attribute location
	// (requires that the shader program has been compiled already!)
	GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
	glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vecPosition);
	
	// get color vertex attribute location
	// (requires that the shader program has been compiled already!)
	GLuint vecColor = glGetAttribLocation(PROGRAM_ID, "vecColor");
	glVertexAttribPointer(vecColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));
	glEnableVertexAttribArray(vecColor);
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// set background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR);
	glBlendColor(1.0f, 1.0f, 1.0f, 1.0f);

	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// get modelview matrix location
	MV_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");
}



void glutResizeCB(int w, int h)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glm::mat4 projection(1.0);
	glm::mat4 camera(1.0);

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// adust perspective projection for new viewport
	GLfloat aspect = (GLfloat)w / (GLfloat)h;

	projection = glm::perspective(glm::radians<float>(91.0), aspect, 0.1f, 20.0f);
	camera = glm::lookAt(glm::vec3(0.0, 0.0, 10.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));

	projection = projection * camera;
	GLint location = glGetUniformLocation(PROGRAM_ID, "matProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
}



void initMenuChange(int entry, char* name, int value)
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
			glPolygonMode(GL_FRONT, GL_LINE);
			initMenuChange(1, "Disable Wireframe", 2);
			break;
		}
		case 2:
		{
			glPolygonMode(GL_FRONT, GL_FILL);
			initMenuChange(1, "Enable Wireframe", 1);
			break;
		}
		case 3:
		{
			if (TRANSPARENCY < 1.0)
			{
				TRANSPARENCY += 0.1f;
				glBlendColor(TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, 1.0f);
			}
			cout << setiosflags(ios::fixed);
			cout << "Transparency: " << setprecision(1) << TRANSPARENCY << endl;
			break;
		}
		case 4:
		{
			if (TRANSPARENCY > 0.0)
			{
				TRANSPARENCY -= 0.1F;
				glBlendColor(TRANSPARENCY, TRANSPARENCY, TRANSPARENCY, 1.0f);
			}
			cout << setiosflags(ios::fixed);
			cout << "Transparency: " << setprecision(1) << TRANSPARENCY << endl;
			break;
		}
		default:
		{
			// reset settings
			glPolygonMode(GL_FRONT, GL_FILL);

			initMenuChange(-1, "Reset Menu", -1);
		}
	}
	glutPostRedisplay();
}



void initMenu()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// work around for FLTK bug in glutChangeToMenuEntry() function
	MENU_ENTRY_STR[0] = ""; // dummy entry since GLUT menus start at index 1
	MENU_ENTRY_STR[1] = "Enable Wireframe";
	MENU_ENTRY_STR[2] = "Increase Alpha [1]";
	MENU_ENTRY_STR[3] = "Decrease Alpha [2]";

	// create menu
	glutCreateMenu(glutMenuCB);
	glutAddMenuEntry(const_cast<char*>(MENU_ENTRY_STR[1].c_str()), 1);
	glutAddMenuEntry(const_cast<char*>(MENU_ENTRY_STR[2].c_str()), 3);
	glutAddMenuEntry(const_cast<char*>(MENU_ENTRY_STR[3].c_str()), 4);
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
		case 27: // ESC
		{
			exit(0);
			break;
		}
		case '1':
		{
			glutMenuCB(3);
			break;
		}
		case '2':
		{
			glutMenuCB(4);
			break;
		}
	}
}



int main(int argc, char *argv[])
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | FL_OPENGL3);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 640);
	glutCreateWindow("Color Cube");

	// register extension wrapper library (GLEW)
	glewExperimental = GL_TRUE; // needed for Apple
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
	glutMenuStatusFunc(glutUpdateMenuCB);
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
		argv[1] = "../../glsl/colorcube.vert";
		argv[2] = "../../glsl/colorcube.frag";
		PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
	}

	// init application
	initRendering();
	initModel(WIDTH);
	initMenu();

	glutMainLoop();
	return 0;  // only for compatibility purposes
}
