///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-04_D.05_Texturing (Ver 2.1)                                                          //
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
#include "../inc/vbosphere.h"
#include "../inc/vbocube.h"
#include "../inc/lodepng.h"


// application global variables and constants /////////////////////////////////////////////////////
GLint PROGRAM_ID = 0;
GLint MV_MAT4_LOCATION = 0;
GLint TEX_MAT4_LOCATION = 0;

int MENU_ENTRY = 0;
int MENU_VALUE = 0;
string MENU_ENTRY_STR[3];

const float WIDTH = 6.0f;	
const float HEIGHT = 5.0f;

enum DRAW_TYPE { DRAW_CUBE, DRAW_SPHERE, DRAW_TEAPOT };
DRAW_TYPE DRAW_MODEL = DRAW_CUBE;

VBOTeapot* TheTeapot = 0;
VBOSphere* TheSphere = 0;
VBOCube* TheCube = 0;

GLuint TEX_NAME[10];
GLuint CURRENT_TEX = 5;

GLuint VA_COLOR_LOCATION = 0;

GLuint GROUND_INDICES_COUNT = 0;
GLuint PYRAMID_INDICES_COUNT = 0;
GLuint PYRAMID_DRAW_OFFSET = 0;


//	create checkerboard texture
#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];



void makeCheckImage()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
   int i, j, c;

   for (i = 0; i < checkImageHeight; i++)
   {
      for (j = 0; j < checkImageWidth; j++)
	  {
         c =  ( ((i & 0x8) == 0) ^ ((j & 0x8) == 0) ) * 255;
         checkImage[i][j][0] = (GLubyte) c;		// red
         checkImage[i][j][1] = (GLubyte) c;		// green
         checkImage[i][j][2] = (GLubyte) c;		// blue
         checkImage[i][j][3] = (GLubyte) 255;	// alpha
      }
   }
}



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

	// bind currently selected texture
	glBindTexture(GL_TEXTURE_2D, TEX_NAME[CURRENT_TEX]);

	switch (DRAW_MODEL)
	{
		case DRAW_CUBE:
		{
			// setup texture matrix
			texture_matrix = glm::scale(texture_matrix, glm::vec3(2.0, -2.0, 2.0));
			glUniformMatrix4fv(TEX_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(texture_matrix));

			// scale cube
			model_matrix = glm::scale(model_matrix, glm::vec3(3.0, 3.0, 3.0));

			// draw cube
			glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model_matrix));
			TheCube->draw();
			break;
		}
		case DRAW_SPHERE:
		{
			// setup texture matrix
			texture_matrix = glm::scale(texture_matrix, glm::vec3(4.0, 4.0, 4.0));
			glUniformMatrix4fv(TEX_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(texture_matrix));

			// scale rotate sphere
			model_matrix = glm::rotate(model_matrix, glm::radians<float>(-90.0), glm::vec3(1.0, 0.0, 0.0));
			model_matrix = glm::scale(model_matrix, glm::vec3(2.2, 2.2, 2.2));

			// draw sphere
			glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model_matrix));
			TheSphere->draw();
			break;
		}
		case DRAW_TEAPOT:
		{
			// setup texture matrix
			texture_matrix = glm::scale(texture_matrix, glm::vec3(1.0, -1.0, 1.0));
			texture_matrix = glm::rotate(texture_matrix, glm::radians<float>(-90.0), glm::vec3(0.0, 0.0, 1.0));
			glUniformMatrix4fv(TEX_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(texture_matrix));

			// move teapot into orgin and rotate 270 degree around x-axis (post multiply order)
			model_matrix = glm::rotate(model_matrix, glm::radians<float>(270.0), glm::vec3(1.0, 0.0, 0.0));
			model_matrix = glm::translate(model_matrix, glm::vec3(0.0, 0.0, -1.5));

			// draw teapot
			glUniformMatrix4fv(MV_MAT4_LOCATION, 1, GL_FALSE, glm::value_ptr(model_matrix));
			TheTeapot->draw();
			break;
		}
	}

	glutSwapBuffers();
	UtilGLSL::checkOpenGLErrorCode();
}



void initModels(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	TheCube = new VBOCube;
	TheSphere = new VBOSphere(1.0f, 60, 30);

	glm::mat4 lidTransform(1.0);
	TheTeapot = new VBOTeapot(16, lidTransform);
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// set background color
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);
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
	TEX_MAT4_LOCATION = glGetUniformLocation(PROGRAM_ID, "matTexture");

	GLint location = glGetUniformLocation(PROGRAM_ID, "uReplaceColor");
	glUniform1i(location, true);

	// load textures from files
	glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
	loadPNGTexture("../../res/yoda_256.png", &TEX_NAME[0]); // Pointer to image data
	loadPNGTexture("../../res/wood_256.png", &TEX_NAME[1]);
	loadPNGTexture("../../res/metal_256.png", &TEX_NAME[2]);
	loadPNGTexture("../../res/stone_256.png", &TEX_NAME[3]);
	loadPNGTexture("../../res/tiles_256.png", &TEX_NAME[4]);


	// setup checkerboard texture image manually
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &TEX_NAME[5]);
	glBindTexture(GL_TEXTURE_2D, TEX_NAME[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
	   checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
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
			initMenuChange(1, "Draw Sphere [Space]", 2);
			DRAW_MODEL = DRAW_CUBE;
			break;
		}
		case 2:
		{
			initMenuChange(1, "Draw Teapot [Space]", 3);
			DRAW_MODEL = DRAW_SPHERE;
			break;
		}		
		case 3:
		{
			initMenuChange(1, "Draw Cube [Space]", 1);
			DRAW_MODEL = DRAW_TEAPOT;
			break;
		}
		case 4:
		{
			CURRENT_TEX = 0;
			break;
		}
		case 5:
		{
			CURRENT_TEX = 1;
			break;
		}
		case 6:
		{
			CURRENT_TEX = 2;
			break;
		}
		case 7:
		{
			CURRENT_TEX = 3;
			break;
		}
		case 8:
		{
			CURRENT_TEX = 4;
			break;
		}
		case 9:
		{
			CURRENT_TEX = 5;
			break;
		}
		case 10:
		{
			// replace material color
			GLint location = glGetUniformLocation(PROGRAM_ID, "uReplaceColor");
			glUniform1i(location, true);

			break;
		}
		case 11:
		{
			// modulate material color
			GLint location = glGetUniformLocation(PROGRAM_ID, "uReplaceColor");
			glUniform1i(location, false);
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
	glutAddMenuEntry("Draw Sphere [Space]", 2);
	glutAddMenuEntry("Texture: Yoda", 4);
	glutAddMenuEntry("Texture: Wood", 5);
	glutAddMenuEntry("Texture: Metal", 6);
	glutAddMenuEntry("Texture: Stone", 7);
	glutAddMenuEntry("Texture: Tiles", 8);
	glutAddMenuEntry("Texture: Checker Board", 9);
 	glutAddMenuEntry("Replace Texture Color", 10);
 	glutAddMenuEntry("Modulate Texture Color", 11);	
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
			DRAW_MODEL = DRAW_TYPE ((DRAW_MODEL+1) % 3);
			glutMenuCB(DRAW_MODEL+1);
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
	glutCreateWindow("Texturing");

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
		argv[1] = "../../glsl/texturing.vert";
		argv[2] = "../../glsl/texturing.frag";
		PROGRAM_ID = UtilGLSL::initShaderProgram(argc, argv);
	}

	// init application
	initRendering();
	initModels();
	initMenu();

	// entering GLUT/FLTK main rendering loop
	glutMainLoop();
	return 0;  // only for compatibility purposes
}
