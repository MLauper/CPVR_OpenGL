///////////////////////////////////////////////////////////////////////////////////////////////////
// Assignment: CG-04-A.03S_YodaCube (Ver 2.1)                                                    //
///////////////////////////////////////////////////////////////////////////////////////////////////



// system includes ////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
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
#include "../inc/vbocube.h"
#include "../inc/lodepng.h"


// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MV_MAT4_LOCATION = 0;
GLint TEX_MAT4_LOCATION = 0;
GLuint TEX_NAME0;
GLuint FRAME_NUM = 0;
const GLuint FRAME_COUNT = 74;
GLuint TEX_FRAMES[FRAME_COUNT];

VBOCube* TheCube = 0;

GLfloat ROTATION = 0.0;



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



void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// clear window background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get trackball transformation matrix
	glm::mat4 mouse = TrackBall::getTransformation();
	glm::mat4 texture_matrix(1.0f);
	glm::mat4 model_matrix(1.0f);
	model_matrix = model_matrix * mouse;

	stack<glm::mat4> tex_matrix_stack;

	model_matrix = glm::scale(model_matrix, glm::vec3(3.0, 3.0, 3.0));


	// textures are upside down --> mirror texture 
	texture_matrix = glm::scale(texture_matrix, glm::vec3(1.0, -1.0, 1.0));
	glUniformMatrix4fv(TEX_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(texture_matrix));
	glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model_matrix));


	// draw cube outside faces //////////////////////////////////////////////////////////
	glFrontFace(GL_CCW);
	glBindTexture(GL_TEXTURE_2D, TEX_NAME0);
	TheCube->draw();


	// draw cube inside faces ///////////////////////////////////////////////////////////
	glFrontFace(GL_CW);
	glBindTexture(GL_TEXTURE_2D, TEX_FRAMES[FRAME_NUM]);

	// show z-axis rotating animated texture
	texture_matrix = glm::translate(texture_matrix, glm::vec3(0.5, 0.5, 0.0));
	texture_matrix = glm::rotate(texture_matrix, glm::radians<float>(ROTATION), glm::vec3(0.0, 0.0, -1.0));
	texture_matrix = glm::translate(texture_matrix, glm::vec3(-0.5, -0.5, 0.0));
	glUniformMatrix4fv(TEX_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(texture_matrix));
	TheCube->draw();

	glutSwapBuffers();
	UtilGLSL::checkOpenGLErrorCode();
}



void initModel(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	TheCube = new VBOCube;
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

	// define fixed light source position and properties
	glm::vec4 position(0.0f, 0.0f, 20.0f, 1.0f);
	glm::vec4  ambient(0.8f, 0.8f,  0.8f, 1.0f);
	glm::vec4  diffuse(0.8f, 0.8f,  0.8f, 1.0f);
	glm::vec4 specular(1.0f, 1.0f,  1.0f, 1.0f);

	// define material properties
	glm::vec4  mat_ambient(0.4f, 0.4f, 0.4f, 1.0f);
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
	TEX_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matTexture");

	GLint location = glGetUniformLocation(PROGRAM_ID, "uReplaceColor");
	glUniform1i(location, true);

	// load textures from files
	glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
	loadPNGTexture("../../res/yoda_512.png", &TEX_NAME0);

	cout << "Loading textures: ";
	stringstream filename;
	for (int i=0; i<FRAME_COUNT; i++)
	{
		filename.str("");
		filename << "../../res/yoda-" << setw(3) << setfill('0') << i << ".png";
		loadPNGTexture((filename.str()).c_str(), &TEX_FRAMES[i]);
		cout << ".";
	}
	cout << endl;
}



void glutTimerCB(int timer_id)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// DEBUG: cout << "glutTimerCB called" << endl;

	ROTATION += 1.0;
    FRAME_NUM++;
    if (FRAME_NUM >= FRAME_COUNT) FRAME_NUM = 0;

    glutPostRedisplay();

    // reset timer
    glutTimerFunc(10, glutTimerCB, 0);
}



void glutResizeCB(int w, int h)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// adust perspective projection for new viewport
	GLfloat aspect = (GLfloat)w / (GLfloat)h;

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
		case 1:
		{
			// modulate material color
			GLint location = glGetUniformLocation(PROGRAM_ID, "uReplaceColor");
			glUniform1i(location, false);
			break;
		}
		case 2:
		{
			// replace material color
			GLint location = glGetUniformLocation(PROGRAM_ID, "uReplaceColor");
			glUniform1i(location, true);
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
	glutAddMenuEntry("Texture Function: GL_MODULATE", 1);
	glutAddMenuEntry("Texture Function: GL_REPLACE", 2);
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
		case '1':
		{
			glutMenuCB(1);
			break;
		}
		case '2':
		{
			glutMenuCB(2);
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
	glutCreateWindow("YodaCube");

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
	glutTimerFunc(10, glutTimerCB, 0);

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
		argv[1] = "../../glsl/texturing.vert";
		argv[2] = "../../glsl/texturing.frag";
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
