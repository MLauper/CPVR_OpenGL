///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-02-D.01 - Hello OpenGL Demo (Ver 2.0)                                                //
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


// application helper includes ////////////////////////////////////////////////////////////////////
#include "../../_COMMON/inc/TrackBall.h"



void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// clear window background
	glClear(GL_COLOR_BUFFER_BIT);

	// apply trackball transformation to modelview matrix
	glLoadIdentity();
	TrackBall::applyTransformation();

	// draw triangle around origin
	glColor3f(0.8f, 0.6f, 0.0f);
	float base = 5.0f;
	glBegin(GL_TRIANGLES);
		glVertex3f(-base, -base, 0.0f); // v0
		glVertex3f( base, -base, 0.0f); // v1
		glVertex3f(    0,  base, 0.0f); // v2
	glEnd();
	
	glutSwapBuffers();
}



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// set background color
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	// setup orthographic projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);

	// setup modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640, 640);
    
	glutCreateWindow("Hello OpenGL");

	// show version of OpenGL
	cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

	// register GLUT/FLTK callbacks
	glutDisplayFunc(glutDisplayCB);
	glutKeyboardFunc(glutKeyboardCB);

	glutMouseFunc(TrackBall::glutMouseButtonCB);
	glutMotionFunc(TrackBall::glutMouseMotionCB);
	glutSpecialFunc(TrackBall::glutSpecialFuncCB);

	// init application 
	initRendering();

	glutMainLoop();
	return 0;  // only for compatibility purposes
}
