///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \title      OpenGL Utility Class
//
//  \module     Utilities
//
//  \filename   UtilOpenGL.cpp
//
//  \brief      General utility class for OpenGL/OS support.
//
//  \subversion
//     Last Subversion commit $Author:$
//     Last Subversion commit   $Date:$
//     Last Subversion File $Revision:$
//
//  \history
//     yyyy-mm-dd   Version   Author   Comment
//     2006-11-06   0.1.00    klu1     initial file release
//     2009-10-02   0.1.01    schar9   Mac OS X compatible, C++ standard conform
//     2015-10-12   0.2.00    klu1     Update for new GLUT/FLTK GUI
//  \endverbatim
*/
///////////////////////////////////////////////////////////////////////////////////////////////////



// system includes ////////////////////////////////////////////////////////////////////////////////
#include <ctime>
#include <iostream>
#include <sstream>


// OpenGL helper includes /////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma warning( disable: 4312 ) // ignore visual studio warnings for FLTK 64-bit type casts
#endif
#include <FL/glut.H>


// application helper includes ////////////////////////////////////////////////////////////////////
#include "../inc/UtilOpenGL.h"
using namespace std;



void UtilOpenGL::showFPS(const string& title)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// replace current GLUT window title with FPS string ("append" can not be implemented,
	// because there is no GLUT function to get the current window title.)

	// calculate frames per second (FPS)
	static int frame = 0;
	static clock_t time = 0;
	static clock_t timebase = 0;
	stringstream title_fps;

	frame++;
	time = std::clock();
	if (time - timebase > CLOCKS_PER_SEC)
	{
		title_fps.setf(std::ios::fixed, std::ios::floatfield);
		title_fps.precision(2);
		title_fps << title << " ( FPS: " << float(frame * CLOCKS_PER_SEC) / (time - timebase) << " )";
		glutSetWindowTitle(const_cast<char*>((title_fps.str()).c_str()));
		timebase = time;
		frame = 0;
	}
}
// UtilOpenGL::showFPS() //////////////////////////////////////////////////////////////////////////

