///////////////////////////////////////////////////////////////////////////////////////////////////
// Assignment: CG-03-A.01S_3DHouse Solution (Ver 2.1)                                            //
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
string MENU_ENTRY_STR[3];

GLenum POLYGON_MODE = GL_FRONT;
GLenum POLYGON_DRAW = GL_FILL;

const float WIDTH = 5.0f;		// ground = width * 2
const float HEIGHT = 3.0f;		// height of roof

GLuint VAO[2];

GLuint VA_COLOR_LOCATION = 0;

GLuint GROUND_INDICES_COUNT = 0;
GLuint FENCE_INDICES_COUNT = 0;
GLuint FENCE_DRAW_OFFSET = 0;

GLuint HOUSE_INDICES_COUNT = 0;
GLuint ROOF_INDICES_COUNT = 0;
GLuint ROOF_DRAW_OFFSET = 0;


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

	bool reenable = false; // local varaibel to re-enable culling 
	
	// make the ground and fence Vertex Array Object current
	glBindVertexArray(VAO[0]);

	// indexed drawing of ground
	glDrawElements(GL_TRIANGLES, GROUND_INDICES_COUNT, GL_UNSIGNED_SHORT, NULL);

	if (glIsEnabled(GL_CULL_FACE)) // fence must not be culled
	{
		glDisable(GL_CULL_FACE);
		reenable = true;
	}

	// indexed drawing of fence
	glDrawElements(GL_TRIANGLE_STRIP, FENCE_INDICES_COUNT, GL_UNSIGNED_SHORT, BUFFER_OFFSET(FENCE_DRAW_OFFSET));

	if (reenable) // re-enable culling for rest of model
	{
		glEnable(GL_CULL_FACE);
		reenable = false;
	}


	// make the house and roof Vertex Array Object current ////////////////////////////////////////
	glBindVertexArray(VAO[1]);

	// disable color vertex attribute to set default color for drawing
	glDisableVertexAttribArray(VA_COLOR_LOCATION);  
	// indexed drawing of house
	glDrawElements(GL_TRIANGLE_STRIP, HOUSE_INDICES_COUNT, GL_UNSIGNED_SHORT, NULL);
	// re-enable color vertex attribute to use color array for drawing
	glEnableVertexAttribArray(VA_COLOR_LOCATION); 
	// indexed drawing of roof
	glDrawElements(GL_TRIANGLE_FAN, ROOF_INDICES_COUNT, GL_UNSIGNED_SHORT, BUFFER_OFFSET(ROOF_DRAW_OFFSET));
	
	glutSwapBuffers();
	UtilGLSL::checkOpenGLErrorCode();
}



void initModel(float witdth, float heigth)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// define ground and fence vertices 
	GLfloat ground_vertices[] =
	{
		-witdth, -witdth,  0.0f, 1.0f,  // v0
		 witdth, -witdth,  0.0f, 1.0f,  // v1
		 witdth,  witdth,  0.0f, 1.0f,  // v2
		-witdth,  witdth,  0.0f, 1.0f,  // v3

		-witdth, -witdth, -2.0f, 1.0f,  // v4
		-witdth,  witdth, -2.0f, 1.0f,  // v5
		 witdth,  witdth, -2.0f, 1.0f,  // v6
		 witdth, -witdth, -2.0f, 1.0f   // v7
	};

	// definition of the colors, each vertex has his own color definition (RGB)
	GLfloat ground_colors[] =
	{
		0.0f, 0.4f, 0.0f,
		0.0f, 0.4f, 0.0f,
		0.0f, 0.4f, 0.0f,
		0.0f, 0.4f, 0.0f,
		0.0f, 0.6f, 0.0f,
		0.0f, 0.7f, 0.0f,
		0.0f, 0.8f, 0.0f,
		0.0f, 0.9f, 0.0f
	};

	// definition of ground face indices (for GL_TRIANGLES --> 6)
	GLushort ground_indices[] =
	{
		0, 3, 2,
		1, 0, 2
	};
	GROUND_INDICES_COUNT = sizeof(ground_indices) / sizeof(ground_indices[0]);
	FENCE_DRAW_OFFSET = sizeof(ground_indices);

	// definition of fence face indices (for GL_TRIANGLE_STRIP --> 10)
	GLushort fence_indices[] =
	{
		0, 4, 1,
		7, 2, 6,
		3, 5,
		0, 4
	};
	FENCE_INDICES_COUNT = sizeof(fence_indices) / sizeof(fence_indices[0]);

	// define house and roof vertices 
	float wall = witdth / 2.0f;
	GLfloat house_vertices[] =
	{
		-wall, -wall,    0.0f, 1.0f,  // v0
		 wall, -wall,    0.0f, 1.0f,  // v1
		 wall,  wall,    0.0f, 1.0f,  // v2
		-wall,  wall,    0.0f, 1.0f,  // v3

		-wall, -wall, -witdth, 1.0f,  // v4
		-wall,  wall, -witdth, 1.0f,  // v5
		 wall,  wall, -witdth, 1.0f,  // v6
		 wall, -wall, -witdth, 1.0f,  // v7

		 0.0f,  0.0f, -witdth -heigth, 1.0f  // v8
	};

	// definition of the colors, each vertex has his own color definition (RGB)
	GLfloat house_colors[] =
	{
		1.0f, 1.0f, 0.0f,
		0.8f, 0.8f, 0.0f,
		0.6f, 0.6f, 0.0f,
		0.4f, 0.4f, 0.0f,
		0.0f, 0.0f, 0.3f,
		0.0f, 0.0f, 0.4f,
		0.0f, 0.0f, 0.5f,
		0.0f, 0.0f, 0.6f,
		0.0f, 0.0f, 1.0f
	};

	// definition of house face indices (for GL_TRIANGLE_STRIP --> 10)
	GLushort house_indices[] =
	{
		0, 4, 1,
		7, 2, 6,
		3, 5,
		0, 4
	};
	HOUSE_INDICES_COUNT = sizeof(house_indices) / sizeof(house_indices[0]);
	ROOF_DRAW_OFFSET = sizeof(house_indices);

	// definition of roof face indices (for GL_TRIANGLE_FAN --> 6)
	GLushort roof_indices[] =
	{
		8, 4, 5, 6, 7, 4
	};
	ROOF_INDICES_COUNT = sizeof(roof_indices) / sizeof(roof_indices[0]);


	// setup global Vertex Array Objects //////////////////////////////////////////////////////////
	glGenVertexArrays(2, &VAO[0]);	

	// setup local Vertex Buffer Objects
	GLuint vbo[2];
	glGenBuffers(2, &vbo[0]);

	// setup local Element Buffer Objects
	GLuint ebo[2];
	glGenBuffers(2, &ebo[0]);

	// get position and color vertex attributes location
	// (requires that the shader program has been compiled already!)
	GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
	VA_COLOR_LOCATION = glGetAttribLocation(PROGRAM_ID, "vecColor");


	// setup Vertex Array Object for ground and fence /////////////////////////////////////////////
	glBindVertexArray(VAO[0]);

	// setup Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices) + sizeof(ground_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ground_vertices), ground_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(ground_vertices), sizeof(ground_colors), ground_colors);

	// setup Vertex Attributes
	glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(VA_COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(ground_vertices)));
	glEnableVertexAttribArray(vecPosition);
	glEnableVertexAttribArray(VA_COLOR_LOCATION);

	// setup Element Buffer Object (Indices)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_indices) + sizeof(fence_indices), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(ground_indices), ground_indices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, FENCE_DRAW_OFFSET, sizeof(fence_indices), fence_indices);


	// setup Vertex Array Object for house and roof ///////////////////////////////////////////////
	glBindVertexArray(VAO[1]);

	// setup Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(house_vertices) + sizeof(house_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(house_vertices), house_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(house_vertices), sizeof(house_colors), house_colors);

	// setup Vertex Attributes
	glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(VA_COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(house_vertices)));
	glVertexAttrib3f(VA_COLOR_LOCATION, 1.0f, 1.0f, 0.0f); // set default draw vertex color if array is disabled
	glEnableVertexAttribArray(vecPosition);
	glEnableVertexAttribArray(VA_COLOR_LOCATION);

	// setup Element Buffer Object (Indices)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(house_indices) + sizeof(roof_indices), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(house_indices), house_indices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, ROOF_DRAW_OFFSET, sizeof(roof_indices), roof_indices);
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// set background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	if (entry > 0) MENU_ENTRY_STR[entry].assign(name);
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
			glEnable(GL_CULL_FACE);
			initMenuChange(2, "Disable Culling", 4);
			break;
		}
		case 4:
		{
			glDisable(GL_CULL_FACE);
			initMenuChange(2, "Enable Culling", 3);
			break;
		}
		case 5:
		{
			glEnable(GL_DEPTH_TEST);
			initMenuChange(3, "Disable Depth Buffer", 6);
			glutSetWindowTitle("3D House (Depth Buffering Enabled)");
			break;
		}
		case 6:
		{
			glDisable(GL_DEPTH_TEST);
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

	// create sub-menu
	sub_menu = glutCreateMenu(glutMenuCB2);
	glutAddMenuEntry("GL_FRONT", GL_FRONT);
	glutAddMenuEntry("GL_BACK", GL_BACK);
	glutAddMenuEntry("GL_FRONT_AND_BACK", GL_FRONT_AND_BACK);

	// work around for FLTK bug in glutChangeToMenuEntry() 
	MENU_ENTRY_STR[0] = ""; // dummy entry since GLUT menus start at index 1
	MENU_ENTRY_STR[1] = "Enable Wireframe";
	MENU_ENTRY_STR[2] = "Enable Culling";
	MENU_ENTRY_STR[3] = "Enable Depth Buffer";


	// create menu
	glutCreateMenu(glutMenuCB1);
	glutAddMenuEntry(const_cast<char*>(MENU_ENTRY_STR[1].c_str()), 1);
	glutAddMenuEntry(const_cast<char*>(MENU_ENTRY_STR[2].c_str()), 3);
	glutAddMenuEntry(const_cast<char*>(MENU_ENTRY_STR[3].c_str()), 5);
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
	}
}



int main(int argc, char *argv[])
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | FL_OPENGL3);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 640);
	glutCreateWindow("3D House");

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
