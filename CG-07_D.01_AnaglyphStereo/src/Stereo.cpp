///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-07_D.01_AnaglyphStereo (Ver 2.1) (Based on an example from Paul Bourke)              //
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


// application helper includes ////////////////////////////////////////////////////////////////////
#include "../../_COMMON/inc/TrackBall.h"
#include "../../_COMMON/inc/UtilGLSL.h"
#include "../inc/vboteapot.h"


// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MV_MAT4_LOCATION = 0;
GLint PC_MAT4_LOCATION = 0;

VBOTeapot* TheTeapot = 0;
GLfloat ASPECT = 1.0;


bool WIREFRAME_MODE = false;
bool ANIMATION_RUNNING = false;
GLfloat ANIMATION_SPEED = 5.0f;
float PHI = 0.0f; // parameter for animation

typedef struct {
	glm::vec3 vp;       // View position           
	glm::vec3 vd;       // View direction vector   
	glm::vec3 vu;       // View up direction       
	glm::vec3 pr;       // Point to rotate about  
	float focallength;  // Focal Length along vd  
	float aperture;     // Camera aperture        
	float eyesep;       // Eye separation         
} CAMERA;

CAMERA TheCamera;


/*/////////////////////////////////////////////////////////////////////////////////////////////////
Move the camera to the home position or to a predefined stereo configuration
The model is assumed to be in a 10x10x10 cube Centered at the origin
/////////////////////////////////////////////////////////////////////////////////////////////////*/
void initCamera(int mode)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	TheCamera.aperture = 20.0f;
	TheCamera.pr = vec3(0.0f, 0.0f,  0.0f);
	TheCamera.vd = vec3(0.0f, 0.0f, -1.0f);
	TheCamera.vu = vec3(0.0f, 1.0f,  0.0f);
	TheCamera.vp = vec3(0.0f, 0.0f, 10.0f);

	switch (mode)
	{
	case 0:
	case 1:
		TheCamera.focallength = 5.0f;
		break;
	case 3:
		TheCamera.focallength = 15.0f;
		break;
	case 4:
		TheCamera.focallength = 10.0f;
		break;
	}

	// non stressful stereo setting
	TheCamera.eyesep = TheCamera.focallength / 30.0f;
	if (mode == 4) TheCamera.eyesep = 0.0f;
}



void drawModel(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	TheTeapot->draw();
}



void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glm::vec3 sep, focus;

	// determine the focal point
	glm::normalize(TheCamera.vd);
	focus = TheCamera.vp + TheCamera.focallength * TheCamera.vd;

	// derive the separation vector for the eye separation
	sep = glm::cross(TheCamera.vd, TheCamera.vu);
	glm::normalize(sep);

	sep *= TheCamera.eyesep / 2.0f;

	// reset color mask and clear window background and depth buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get trackball transformation matrix
	glm::mat4 mouse = TrackBall::getTransformation();
	glm::mat4 model(1.0f);
	stack<glm::mat4> model_view_stack;


	// move teapot into orgin and rotate 270 degree around x-axis (post multiply order)
	glm::mat4 teapot_matrix(1.0);
	teapot_matrix = glm::rotate(teapot_matrix, glm::radians<float>(270.0), glm::vec3(1.0, 0.0, 0.0));
	teapot_matrix = glm::translate(teapot_matrix, glm::vec3(0.0, 0.0, -1.5));


	// start drawing red left eye stereo image //////////////////////////////////////////
	// set left eye filter mask for RED-blue glasses
	glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);

	// set projection and left eye
	glm::mat4 projection = glm::perspective(TheCamera.aperture, ASPECT, 1.0f, 20.0f);
	glm::mat4 lefteye = glm::lookAt(TheCamera.vp - sep, focus, TheCamera.vu);
	projection = projection * lefteye;
	glUniformMatrix4fv(PC_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(projection));

	model_view_stack.push(model);
		model = glm::translate(model, glm::vec3(0.0, 0.0, 0.02f * PHI));
		model = model * mouse;
		model = model * teapot_matrix;
		glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));
		drawModel();
		model = model_view_stack.top();
	model_view_stack.pop();


	// start drawing green right eye stereo image ///////////////////////////////////////
	// clear depth buffer for right eye image
	glClear(GL_DEPTH_BUFFER_BIT);

	// set right eye filter mask for red-BLUE glasses
	glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);

	// set projection and right eye camera
	projection = glm::perspective(TheCamera.aperture, ASPECT, 1.0f, 20.0f);
	glm::mat4 righteye = glm::lookAt(TheCamera.vp + sep, focus, TheCamera.vu);
	projection = projection * righteye;
	glUniformMatrix4fv(PC_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(projection));

	model_view_stack.push(model);
		model = glm::translate(model, glm::vec3(0.0, 0.0, 0.02f * PHI));
		model = model * mouse;
		model = model * teapot_matrix;
		glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model));
		drawModel();
		model = model_view_stack.top();
	model_view_stack.pop();

	// show left and right stereo image in back buffer
	glutSwapBuffers();
	UtilGLSL::checkOpenGLErrorCode();
}



void animateScene(int timer_id)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// update animation parameter
	if (PHI > 180.0) ANIMATION_SPEED = -1.0f * ANIMATION_SPEED;
	if (PHI < -180.0) ANIMATION_SPEED = -1.0f * ANIMATION_SPEED;

	PHI = PHI + ANIMATION_SPEED;

	// redraw the scene
	glutPostRedisplay();

	// reset timer if the animation is running
	if (ANIMATION_RUNNING) glutTimerFunc(1, animateScene, 0);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// define fixed light source position and properties
	glm::vec4 position(0.0f, 0.0f, 20.0f, 1.0f);
	glm::vec4  ambient(1.0f, 1.0f,  1.0f, 1.0f);
	glm::vec4  diffuse(1.0f, 1.0f,  1.0f, 1.0f);
	glm::vec4 specular(1.0f, 1.0f,  1.0f, 1.0f);

	// define material properties
	glm::vec4  mat_ambient(0.2f, 0.2f, 0.2f, 1.0f);
	glm::vec4  mat_diffuse(0.8f, 0.8f, 0.8f, 1.0f);
	glm::vec4 mat_specular(0.2f, 0.2f, 0.2f, 1.0f);
	GLfloat  mat_shininess = 128.0f;

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
	ASPECT = (GLfloat)w / (GLfloat)h;

	glm::mat4 projection = glm::perspective(20.0f, ASPECT, 1.0f, 20.0f);
	glm::mat4 camera = glm::lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));

	projection = projection * camera;
	PC_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matProjection");
	glUniformMatrix4fv(PC_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(projection));
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
			glutChangeToMenuEntry(1, "Stop Animation (Space)", 2);
			ANIMATION_RUNNING = true;
			glutTimerFunc(1, animateScene, 0);
			break;
		}
		case 2:
		{
			glutChangeToMenuEntry(1, "Start Animation (Space)", 1);
			ANIMATION_RUNNING = false;
			break;
		}
		case 3:
		{
			// increase animation speed
			if (ANIMATION_SPEED >= 0.0f)
			{
				ANIMATION_SPEED += 0.1f;
			}
			else
			{
				ANIMATION_SPEED -= 0.1f;
			}
			break;
		}
		case 4:
		{
			// decrease animation speed
			if (ANIMATION_SPEED < 0.0f)
			{
				ANIMATION_SPEED += 0.1f;
				if (abs(ANIMATION_SPEED) < 0.1) ANIMATION_SPEED = -0.1f;
			}
			else
			{
				ANIMATION_SPEED -= 0.1f;
				if (abs(ANIMATION_SPEED) < 0.1) ANIMATION_SPEED = 0.1f;
			}
			break;
		}
		case 5:
		{
			if (WIREFRAME_MODE)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			WIREFRAME_MODE = !WIREFRAME_MODE;
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

	glutAddMenuEntry("Start Animation (Space)", 1);
	glutAddMenuEntry("Increase Speed (Key 1)", 3);
	glutAddMenuEntry("Decrease Speed (Key 2)", 4);
	glutAddMenuEntry("Toggle Wireframe Mode", 5);
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
		case ' ':
		{
			if (ANIMATION_RUNNING)
			{
				glutMenuCB(2);
			}
			else {
				glutMenuCB(1);
			}
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

	glutPostRedisplay();
}



int main(int argc, char *argv[])
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | FL_OPENGL3);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(900, 640);
	glutCreateWindow("Anaglyph Stereo");

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
	initCamera(1);
	initMenu();

	// entering GLUT/FLTK main rendering loop
	glutMainLoop();
	return 0;  // only for compatibility purposes
}
