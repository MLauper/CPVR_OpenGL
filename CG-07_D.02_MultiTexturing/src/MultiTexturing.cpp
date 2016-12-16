///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-07_02D - Single-/Multi-Texture GLSL Demo(Ver 2.1)                                    //
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
#include "../inc/vbosphere.h"
#include "../inc/lodepng.h"


// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MV_MAT4_LOCATION = 0;
GLint MVL_MAT4_LOCATION = 0;
GLint TEX_MAT4_LOCATION = 0;

GLuint TEX_NAME[4];
GLuint CURRENT_TEX = 0;

VBOSphere* TheSphere = 0;

bool  ANIMATION_RUNNING = false;
float ANGLE = 0.0;
int   SPEED = 20;



void loadPNGTexture(const char* filename, GLuint* id)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// load file and decode image.
	vector<unsigned char> image;
	unsigned int width, height;
	unsigned int error = lodepng::decode(image, width, height, filename);

	// if there's an error, display it.
	if (error != 0) cout << endl << "loadPNGTexture Error " << error << ": " << lodepng_error_text(error) << endl;

	glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, *id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
}



void drawSphere(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	TheSphere->draw();
}



void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// clear window background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get trackball transformation matrix
	glm::mat4 mouse = TrackBall::getTransformation();
	glm::mat4 model(1.0f);
	model = model * mouse;

	// turn earth rotation axis straight
	model = glm::rotate(model, glm::radians<float>(-90.0), glm::vec3(1.0, 0.0, 0.0));
	glUniformMatrix4fv(MVL_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));

	// draw sphere with texture coordinates
	model = glm::rotate(model, glm::radians<float>(ANGLE), glm::vec3(0.0, 0.0, 1.0));
	glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));
	TheSphere->draw();

	glutSwapBuffers();
}



void initModel(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	TheSphere = new VBOSphere(1.2f, 60, 30);
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// set background color
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// load textures from files
	glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
	loadPNGTexture("../../res/EarthDay.png", &TEX_NAME[0]);
	loadPNGTexture("../../res/EarthNight.png", &TEX_NAME[1]);
	loadPNGTexture("../../res/EarthCloudsGloss.png", &TEX_NAME[2]);

	// assign textures to texture units
	GLint location = glGetUniformLocation(PROGRAM_ID, "EarthDay");
	glUniform1i(location, 0);

	location = glGetUniformLocation(PROGRAM_ID, "EarthNight");
	glUniform1i(location, 1);

	location = glGetUniformLocation(PROGRAM_ID, "EarthCloudGloss");
	glUniform1i(location, 2);

	location = glGetUniformLocation(PROGRAM_ID, "NoClouds");
	glUniform1i(location, false);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX_NAME[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEX_NAME[1]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TEX_NAME[2]);

	// define fixed light source position and get location
	glm::vec4 position( 40.0, 1.0,  10.0, 1.0);
	location = glGetUniformLocation(PROGRAM_ID, "vecLightPosition");
	glUniform4fv(location, 1, glm::value_ptr(position));

	// get modelview matrix location
	MV_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelView");
	MVL_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matModelViewLight");
	TEX_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matTexture");

	// setup texture matrix
	TEX_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matTexture");
	glm::mat4 texture_matrix(1.0);
	glUniformMatrix4fv(TEX_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(texture_matrix));
}



void glutResizeCB(int w, int h)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// adust aspect value for new viewport
	GLfloat aspect = (GLfloat)w / (GLfloat)h;

	// adust perspective projection for new viewport 
	glm::mat4 projection = glm::perspective(glm::radians<float>(20.0f), aspect, 1.0f, 20.0f);
	glm::mat4 camera = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	projection = projection * camera;
	GLint location = glGetUniformLocation(PROGRAM_ID, "matProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projection));
}



void timer(int timer_id)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	ANGLE += 0.4f;

	// reset timer if the animation is running
	if (ANIMATION_RUNNING) glutTimerFunc(SPEED, timer, 0);

	glutPostRedisplay();
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
			glGetIntegerv(GL_CURRENT_PROGRAM, &PROGRAM_ID);
			GLint location = glGetUniformLocation(PROGRAM_ID, "NoClouds");

			GLint no_clouds = true; // GLboolean is not accepted by the compiler
			glGetUniformiv(PROGRAM_ID, location, &no_clouds);
			glUniform1i(location, !no_clouds);
			break;
		}
		case 2:
		{
			ANIMATION_RUNNING = !ANIMATION_RUNNING;
			if (ANIMATION_RUNNING)
			{
				glutTimerFunc(SPEED,  timer, 0);
			}
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
    glutAddMenuEntry("Toggle Clouds On/OFF [Key c]", 1);
    glutAddMenuEntry("Toggle Animation On/OFF [Space]", 2);
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
		case 'c': // toggle clouds
		{
			glutMenuCB(1);
			break;
		}
		case ' ': // space key
		{
			glutMenuCB(2);
			break;
		}
		default:
		{
			glutMenuCB(key-48); // subtract '\0'
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
	glutCreateWindow("MultiTexturing");

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
		argv[1] = "../../glsl/multitex.vert";
		argv[2] = "../../glsl/multitex.frag";
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

