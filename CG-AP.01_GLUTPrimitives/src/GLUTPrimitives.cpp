///////////////////////////////////////////////////////////////////////////////////////////////////
// Demo: CG-AP.01 GLUT Drawing Primitives (Ver. 2.0)                                             //
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


bool WIREFRAME = false;
bool SHOW_ALL = false;

enum GLUT_PRIMITIVE_TYPE {
	GP_ALL = 5,
	GP_TETRAHEDRON,
	GP_CUBE,
	GP_OCTAHEDRON,
	GP_DODECAHEDRON,
	GP_ICOSAHEDRON,
	GP_CONE,
	GP_SPHERE,
	GP_TORUS,
	GP_TEAPOT
};

GLUT_PRIMITIVE_TYPE GLUT_PRIMITIVE = GP_TEAPOT;


void drawGlutPrimitives()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	GLdouble radius = 0.7;
	GLdouble height = radius;
	GLdouble outRadius = 0.7 * radius;
	GLdouble inRadius = 0.3 * radius;
	GLdouble size = 1.45 * radius;
	GLint nsides = 30;
	GLint rings = 60;

	switch (GLUT_PRIMITIVE)
	{
	case GP_ALL:
	case GP_TETRAHEDRON:
	{
		if(SHOW_ALL)
		{
			glPushMatrix();
			TrackBall::applyTransformation();
			glScalef(0.8F, 0.8F, 0.8F);
			if (WIREFRAME) glutWireTetrahedron(); else glutSolidTetrahedron();
			glPopMatrix();
			glTranslatef(1.5, 0.0, 0.0);
		}
		else
		{
			if (WIREFRAME) glutWireTetrahedron(); else glutSolidTetrahedron();
			break;
		}
	}
	case GP_CUBE:
	{
		if(SHOW_ALL)
		{
			glPushMatrix();
			TrackBall::applyTransformation();
			if (WIREFRAME) glutWireCube(size); else glutSolidCube(size);
			glPopMatrix();
			glTranslatef(1.5, 0.0, 0.0);
		}
		else
		{
			if (WIREFRAME) glutWireCube(size); else glutSolidCube(size);
			break;
		}
	}
	case GP_OCTAHEDRON:
	{
		if(SHOW_ALL)
		{
			glPushMatrix();
			TrackBall::applyTransformation();
			glScalef(0.8F, 0.8F, 0.8F);
			if (WIREFRAME) glutWireOctahedron(); else glutSolidOctahedron();
			glPopMatrix();
			glTranslatef(1.5, 0.0, 0.0);
		}
		else
		{
			if (WIREFRAME) glutWireOctahedron(); else glutSolidOctahedron();
			break;
		}
	}
	case GP_DODECAHEDRON:
	{
		if(SHOW_ALL)
		{
			glPushMatrix();
			TrackBall::applyTransformation();
			glScalef(0.4F, 0.4F, 0.4F);
			if (WIREFRAME) glutWireDodecahedron(); else glutSolidDodecahedron();
			glPopMatrix();
			glTranslatef(1.5, 0.0, 0.0);
		}
		else
		{
			if (WIREFRAME) glutWireDodecahedron(); else glutSolidDodecahedron();
			break;
		}
	}
	case GP_ICOSAHEDRON:
	{
		if(SHOW_ALL)
		{
			glPushMatrix();
			TrackBall::applyTransformation();
			glScalef(0.8F, 0.8F, 0.8F);
			if (WIREFRAME) glutWireIcosahedron(); else glutSolidIcosahedron();
			glPopMatrix();
			glTranslatef(1.5, 0.0, 0.0);
		}
		else
		{
			if (WIREFRAME) glutWireIcosahedron(); else glutSolidIcosahedron();
			break;
		}
	}
	case GP_CONE:
	{
		if(SHOW_ALL)
		{
			glPushMatrix();
			TrackBall::applyTransformation();
			if (WIREFRAME) glutWireCone(radius, height, 40, 10); else glutSolidCone(radius, height, 40, 10);
			glPopMatrix();
			glTranslatef(1.5, 0.0, 0.0);
		}
		else
		{
			if (WIREFRAME) glutWireCone(radius, height, 40, 10); else glutSolidCone(radius, height, 40, 10);
			break;
		}
	}
	case GP_SPHERE:
	{
		if(SHOW_ALL)
		{
			glPushMatrix();
			TrackBall::applyTransformation();
			if (WIREFRAME) glutWireSphere(radius, 40, 20); else glutSolidSphere(radius, 40, 20);
			glPopMatrix();
			glTranslatef(1.5, 0.0, 0.0);
		}
		else
		{
			if (WIREFRAME) glutWireSphere(radius, 40, 20); else glutSolidSphere(radius, 40, 20);
			break;
		}
	}
	case GP_TORUS:
	{
		if(SHOW_ALL)
		{
			glPushMatrix();
			TrackBall::applyTransformation();
			if (WIREFRAME) glutWireTorus(inRadius, outRadius, nsides, rings); else glutSolidTorus(inRadius, outRadius, nsides, rings);
			glPopMatrix();
			glTranslatef(1.5, 0.0, 0.0);
		}
		else
		{
			if (WIREFRAME) glutWireTorus(inRadius, outRadius, nsides, rings); else glutSolidTorus(inRadius, outRadius, nsides, rings);
			break;
		}
	}
	case GP_TEAPOT:
	{
		if(SHOW_ALL)
		{
			glPushMatrix();
			TrackBall::applyTransformation();
			glFrontFace(GL_CW);  // teapot face ordering is reversed
			if (WIREFRAME) glutWireTeapot(radius);  else  glutSolidTeapot(radius);
			glFrontFace(GL_CCW); // restore normal face ordering
			glPopMatrix();
		}
		else
		{
			glFrontFace(GL_CW);  // teapot face ordering is reversed
			if (WIREFRAME) glutWireTeapot(radius);  else  glutSolidTeapot(radius);
			glFrontFace(GL_CCW); // restore normal face ordering
			break;
		}
	}
	} // end switch
}
///////////////////////////////////////////////////////////////////////////////////////////////////



void glutDisplayCB(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// clear window background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	if (SHOW_ALL)
	{
		glPushMatrix();
			WIREFRAME = true;
			glTranslatef(-6.25, 1.0, 0.0);
			drawGlutPrimitives();
		glPopMatrix();

		glPushMatrix();
			WIREFRAME = false;
			glTranslatef(-6.25, -1.0, 0.0);
			drawGlutPrimitives();
		glPopMatrix();
	}
	else
	{
		// apply trackball transformation to modelview matrix
		glLoadIdentity();
		TrackBall::applyTransformation();
		drawGlutPrimitives();
	}
	glPopMatrix();

	glutSwapBuffers();
}
///////////////////////////////////////////////////////////////////////////////////////////////////



void initRendering()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// set background color
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// general init
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glShadeModel( GL_SMOOTH );


	// setup light parameters
	GLfloat lightPosition[] = {-3.0F, 0.0F, 1.5F, 1.0F};
	GLfloat matSpecular[] = {0.8F, 0.8F, 0.8F, 1.0F};
	GLfloat matShininess[] = {80.0F};

	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	// force surface normal re-normalization due to scale transformation
	glEnable(GL_NORMALIZE);

	// use material color settings
	glColor3f(0.0F, 0.2F, 1.0F);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// use anti-aliasing
	//glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);

	// set line width to smallest anti-aliased value
	float range[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE, range);
	glLineWidth(range[0]);
}
///////////////////////////////////////////////////////////////////////////////////////////////////



void glutKeyboardCB(unsigned char key, int x, int y)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// ToDo: implement custom keyboard callback
	if ((key == 'q') || (key == 'Q') || (key == '\033') ) exit(0);
}


void reshape(int width, int height)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(50.0, (GLfloat) width / (GLfloat) height, 1.0, 9.0);
	glOrtho(-7.0, 7.0, -4.0, 4.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);

	glutPostRedisplay();
}





void MenuHandler(int mode)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	SHOW_ALL = false;

	switch(mode)
	{
	case 0:
		exit(0);
		break;
	case 1:
		WIREFRAME = !WIREFRAME;
	    break;
	case GP_ALL:
		SHOW_ALL = true;
	default:
		GLUT_PRIMITIVE = static_cast<GLUT_PRIMITIVE_TYPE>(mode);
	    break;
	}
	glutPostRedisplay();
}
///////////////////////////////////////////////////////////////////////////////////////////////////



void initMenu()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// create main menu
	glutCreateMenu(MenuHandler);
	glutAddMenuEntry("Toggle Wireframe", 1);

	glutAddMenuEntry("Show All", GP_ALL);
	glutAddMenuEntry("Tetrahedron", GP_TETRAHEDRON);
	glutAddMenuEntry("Cube", GP_CUBE);
	glutAddMenuEntry("Octahedron", GP_OCTAHEDRON);
	glutAddMenuEntry("Dodecahedron", GP_DODECAHEDRON);
	glutAddMenuEntry("Icosahedron", GP_ICOSAHEDRON);
	glutAddMenuEntry("Cone", GP_CONE);
	glutAddMenuEntry("Sphere", GP_SPHERE);
	glutAddMenuEntry("Torus", GP_TORUS);
	glutAddMenuEntry("Teapot", GP_TEAPOT);

	glutAddMenuEntry("Exit", 0);

	// attach menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}



int main(int argc, char *argv[])
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
 	glutInitWindowPosition(50, 100);
	glutInitWindowSize(1400, 800);
	glutCreateWindow("GLUT Graphic Primitives");

	// show version of OpenGL
	cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

	// register GLUT/FLTK callbacks
	glutDisplayFunc(glutDisplayCB);
	glutKeyboardFunc(glutKeyboardCB);
	glutReshapeFunc(reshape);

	glutMouseFunc(TrackBall::glutMouseButtonCB);
	glutMotionFunc(TrackBall::glutMouseMotionCB);
	glutSpecialFunc(TrackBall::glutSpecialFuncCB);

	// init application
	initRendering();
	initMenu();

	glutMainLoop();
	return 0;  // only for compatibility purposes
}
///////////////////////////////////////////////////////////////////////////////////////////////////