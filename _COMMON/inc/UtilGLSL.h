///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \filename   UtilGLSL.h
//
//  \brief      Implementation of GLSL utility class for OpenGL GLSL support
//
//
//  \history
//     yyyy-mm-dd   Version   Author   Comment
//     2015-09-24   1.30      klu      Update for new shader based CG course
//     2014-11-27   1.10      klu      Initial file release
//  \endverbatim
*/
///////////////////////////////////////////////////////////////////////////////////////////////////



// system includes ////////////////////////////////////////////////////////////////////////////////
#include <string>


class UtilGLSL
///////////////////////////////////////////////////////////////////////////////////////////////////
{
public:
	static void   showOpenGLVersion(void);
	static void   showGLSLVersion(void);
	static float  checkOpenGLVersion(void);
	static void   initOpenGLDebugCallback(void);
	static void   checkOpenGLErrorCode(void);
	static void   checkShaderInfoLog(GLuint shader);
	static void   checkProgramInfoLog(GLuint program);
	static GLuint initShaderProgram(int argc, char **argv);

private:
	static char*  readShaderFile(const string& filename);
	static void   DebugMessageCallback(GLenum source, GLenum type, GLuint id,
					GLenum severity, GLsizei length, const GLchar* message,	void* userParam);
};
// class UtilGLSL /////////////////////////////////////////////////////////////////////////////////