///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \title      OpenGL Utility Class
//
//  \module     Utilities
//
//  \filename   UtilOpenGL.h
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
#include <string>



class UtilOpenGL
{
public:
	static void showFPS(const std::string& title = " ");
};
