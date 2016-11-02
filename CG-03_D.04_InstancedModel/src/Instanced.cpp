///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-03-D.04_InstancedRendering (Ver 2.1)                                                //
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
#include "../inc/UtilOpenGL.h"


// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MV_MAT4_LOCATION = 0;
GLuint VA_COLOR_LOCATION = 0;
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

const float WIDTH = 5.0f;
const float HEIGHT = 5.0f;
const int DRAW_MAX = 50;
bool INSTANCED_DRAW = false;
bool PERSPECTIVE_DRAW = false;
int COUNTER = 0;

string WINDOW_TITLE = "Non-Instanced Rendering";
int MENU_ENTRY = 0;
int MENU_VALUE = 0;
string MENU_ENTRY_STR[3];

GLuint GROUND_INDICES_COUNT = 0;
GLuint PYRAMID_INDICES_COUNT = 0;
GLuint PYRAMID_DRAW_OFFSET = 0;



void drawPyramid(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// disable color vertex attribute to set default color for ground drawing
	glDisableVertexAttribArray(VA_COLOR_LOCATION);
	// indexed drawing of ground
	if (INSTANCED_DRAW) {
		glDrawElementsInstanced(GL_TRIANGLES, GROUND_INDICES_COUNT, GL_UNSIGNED_SHORT, NULL, DRAW_MAX);
	} else {
		glDrawElements(GL_TRIANGLES, GROUND_INDICES_COUNT, GL_UNSIGNED_SHORT, NULL);
	}
	// re-enable color vertex attribute to use color array for pyramid drawing
	glEnableVertexAttribArray(VA_COLOR_LOCATION);
	// indexed drawing of pyramid
	if (INSTANCED_DRAW) {
		glDrawElementsInstanced(GL_TRIANGLE_FAN, PYRAMID_INDICES_COUNT, GL_UNSIGNED_SHORT, BUFFER_OFFSET(PYRAMID_DRAW_OFFSET), DRAW_MAX);
	} else {
		glDrawElements(GL_TRIANGLE_FAN, PYRAMID_INDICES_COUNT, GL_UNSIGNED_SHORT, BUFFER_OFFSET(PYRAMID_DRAW_OFFSET));
	}

	COUNTER++;
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

	// draw pyramids with offset around origin
	float offset = WIDTH / 2.0f;
	float initial_offset = DRAW_MAX / 2.0f;
	COUNTER = 0;

	// rotate coordinate system 90 degree around x-axis and move inital offset
	model = glm::rotate(model, glm::radians<float>(90.0), glm::vec3(1.0f, 0.0f, 0.0f));
	model = model * view;
	model = glm::translate(model, glm::vec3(-initial_offset*offset, -initial_offset*offset, -initial_offset*offset ));

	model_view_stack.push(model);
	for (int i = 0; i < DRAW_MAX; i++)
	{
		for (int j = 0; j < DRAW_MAX; j++)
		{
			if (INSTANCED_DRAW)
			{
				// let the vertex shader draw the remaining z-axis pyramid instances
				model = glm::translate(model, glm::vec3(i*offset, j*offset, 0.0f));
				glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));
				drawPyramid();
				model = model_view_stack.top();
			}
			else
			{
				// draw the remaining pyramids in z-axis direction ourselfes
				for (int k = 0; k < DRAW_MAX; k++)
				{
					model = glm::translate(model, glm::vec3(i*offset, j*offset, k*offset));
					glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));
					drawPyramid();
					model = model_view_stack.top();
				}
			}
		}
	}
	model_view_stack.pop();

	glutSwapBuffers();
	UtilGLSL::checkOpenGLErrorCode();
	UtilOpenGL::showFPS(WINDOW_TITLE.c_str());
}



void initModel(float witdth, float heigth)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	float base = witdth / 2.0F;

	// define pyramid vertices /////////////////////////////////////////////////////////////
	GLfloat pyramid_vertices[] =
	{
		-base, -base, 0.0f, 1.0f,  // v0
		 base, -base, 0.0f, 1.0f,  // v1
		 base,  base, 0.0f, 1.0f,  // v2
		-base,  base, 0.0f, 1.0f,  // v3

		 0.0f,  0.0f,  -heigth, 1.0f  // v4
	};

	// definition of the colors, each vertex has his own color definition (RGB)
	GLfloat pyramid_colors[] =
	{
		0.0f, 0.0f, 0.2f,
		0.0f, 0.0f, 0.4f,
		0.0f, 0.0f, 0.6f,
		0.0f, 0.0f, 0.8f,
		0.0f, 0.0f, 1.0f
	};

	// definition of ground face indices (for GL_TRIANGLES --> 6)
	GLushort ground_indices[] =
	{
		0, 1, 2,
		3, 0, 2
	};
	GROUND_INDICES_COUNT = sizeof(ground_indices) / sizeof(ground_indices[0]);
	PYRAMID_DRAW_OFFSET = sizeof(ground_indices);

	// definition of pyramid face indices (for GL_TRIANGLE_FAN --> 6)
	GLushort pyramid_indices[] =
	{
		4, 0, 3, 2, 1, 0
	};
	PYRAMID_INDICES_COUNT = sizeof(pyramid_indices) / sizeof(pyramid_indices[0]);


	// setup Vertex Array Object //////////////////////////////////////////////////////////////////
	GLuint vao;
	glGenVertexArrays(1, &vao);

	// setup local Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// setup local Index Buffer Object
	GLuint ibo;
	glGenBuffers(1, &ibo);

	// get position and color vertex attributes location
	// (requires that the shader program has been compiled already!)
	GLuint vecPosition = glGetAttribLocation(PROGRAM_ID, "vecPosition");
	VA_COLOR_LOCATION = glGetAttribLocation(PROGRAM_ID, "vecColor");


	// setup Vertex Array Object for ground and pyramid ///////////////////////////////////////////
	glBindVertexArray(vao);

	// setup Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices) + sizeof(pyramid_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pyramid_vertices), pyramid_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), sizeof(pyramid_colors), pyramid_colors);

	// setup Vertex Attributes
	glVertexAttribPointer(vecPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(VA_COLOR_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pyramid_vertices)));
	glVertexAttrib3f(VA_COLOR_LOCATION, 0.4f, 0.4f, 1.0f); // set default draw vertex color if array is disabled
	glEnableVertexAttribArray(vecPosition);
	glEnableVertexAttribArray(VA_COLOR_LOCATION);

	// setup Indices Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_indices) + sizeof(pyramid_indices), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(ground_indices), ground_indices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, PYRAMID_DRAW_OFFSET, sizeof(pyramid_indices), pyramid_indices);
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// set background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	GLint location = glGetUniformLocation(PROGRAM_ID, "floatOffset");
	glUniform1f(location, HEIGHT);

	// get modelview matrix location
	MV_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");
}



void glutResizeCB(int w, int h)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glm::mat4 projection(1.0);
	glm::mat4 camera(1.0);


	if (PERSPECTIVE_DRAW)
	{
		// adust perspective projection for new viewport
		GLfloat aspect = (GLfloat)w / (GLfloat)h;

		projection = glm::perspective(glm::radians<float>(79.0), aspect, 0.1f, 240.0f);
		camera = glm::lookAt(glm::vec3(0.0, 0.0, 200.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
	}
	else
	{
		// adust orthographic projection for new viewport
		GLfloat aspect = (GLfloat)w / (GLfloat)h;
		GLfloat left   = -120.0, right = 120.0;
		GLfloat bottom = -120.0, top   = 120.0;
		GLfloat znear  = -120.0, zfar  = 120.0;

		if (aspect > 1.0f)
		{
			left *= aspect;
			right *= aspect;
		}
		else {
			bottom /= aspect;
			top /= aspect;
		}

		projection = glm::ortho(left, right, bottom, top, znear, zfar);
		camera = glm::lookAt(glm::vec3(0.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
	}

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
			initMenuChange(1, "Use Non-Instanced Rendering", 2);
			cout << "Non-Instanced Model Rendering: " << COUNTER << " drawPyramid() calls executed, switching..." << endl;
			WINDOW_TITLE = "Instanced Model Rendering";
			INSTANCED_DRAW = true;
			break;
		}
		case 2:
		{
			initMenuChange(1, "Use Instanced Rendering", 1);
			cout << "    Instanced Model Rendering: " << COUNTER << " drawPyramid() calls executed, switching..." << endl;
			WINDOW_TITLE = "Non-Instanced Model Rendering";
			INSTANCED_DRAW = false;
			break;
		}
		case 3:
		{
			initMenuChange(2, "Use Ortographic Projection", 4);
			PERSPECTIVE_DRAW = true;
			glutResizeCB(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			break;
		}
		case 4:
		{
			initMenuChange(2, "Use Perspective Projection", 3);
			PERSPECTIVE_DRAW = false;
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
	// work around for FLTK bug in glutChangeToMenuEntry() function
	MENU_ENTRY_STR[0] = ""; // dummy entry since GLUT menus start at index 1
	MENU_ENTRY_STR[1] = "Use Instanced Rendering";
	MENU_ENTRY_STR[2] = "Use Perspective Projection";

	// create menu
	glutCreateMenu(glutMenuCB);
	glutAddMenuEntry(const_cast<char*>(MENU_ENTRY_STR[1].c_str()), 1);
	glutAddMenuEntry(const_cast<char*>(MENU_ENTRY_STR[2].c_str()), 3);
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
		case 'i': case 'I':
		{
			if (INSTANCED_DRAW)
			{
				cout << "    Instanced Model Rendering: " << COUNTER << " drawPyramid() calls executed, switching..., switching..." << endl;
				WINDOW_TITLE = "Non-Instanced Model Rendering";
			} else
			{
				cout << "Non-Instanced Model Rendering: " << COUNTER << " drawPyramid() calls executed, switching..., switching..."  << endl;
				WINDOW_TITLE = "Instanced Model Rendering";
			}
			INSTANCED_DRAW = !INSTANCED_DRAW;
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
	glutCreateWindow("Instanced Model Rendering");

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
	glutReshapeFunc(glutResizeCB);
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
		argv[1] = "../../glsl/instanced.vert";
		argv[2] = "../../glsl/instanced.frag";
		PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
	}

	// init application
	initRendering();
	initModel(WIDTH, HEIGHT);
	initMenu();

	// entering GLUT/FLTK main rendering loop
	glutMainLoop();
	return 0;  // only for compatibility purposes
}
