///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \filename   UtilGLSL.cpp
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
#include <iostream>
#include <fstream>
#include <string>
#include <typeinfo>
using namespace std;



// OpenGL helper includes /////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>



// application helper includes ////////////////////////////////////////////////////////////////////
#include "../inc/UtilGLSL.h"



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: showOpenGLVersion()
// purpose:  Function to show the OpenGL implementation and version.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilGLSL::showOpenGLVersion(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);

	cout << "OpenGL Vendor : " << (char*)vendor << " (" << (char*)renderer << ")" << endl << endl;
	cout << "OpenGL Version: " << (char*)version << endl;
}
// showOpenGLVersion() ////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: showGLSLVersion()
// purpose:  Function to show the supported GLSL version.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilGLSL::showGLSLVersion()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	cout << "  GLSL Version: " << (char*)glslVersion << endl << endl;
}
// showGLSLVersion() //////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: checkOpenGLVersion()
// purpose:  Function to return the OpenGL version as a float number for version checking.
///////////////////////////////////////////////////////////////////////////////////////////////////
float UtilGLSL::checkOpenGLVersion()
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	GLint gl_major, gl_minor;
	glGetIntegerv(GL_MAJOR_VERSION, &gl_major);
	glGetIntegerv(GL_MINOR_VERSION, &gl_minor);
	//cout << "DEBUG: OpenGL Version: " << *major << "." << *minor << endl;

	return gl_major + (0.1f * gl_minor);
}
// checkOpenGLVersion() ///////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: DebugMessageCallback()
// purpose:  The glDebugMessageCallback() function to be registered by initOpenGLDebugCallback().
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilGLSL::DebugMessageCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message, void* userParam)
{
	cout << "OpenGL Debug Callback: " << message << endl;
	cout << "(Source: ";
	switch (source)
	{
		case GL_DEBUG_SOURCE_API: cout << "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:	cout << "WINDOW_SYSTEM"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: cout << "THIRD_PARTY"; break;
		case GL_DEBUG_SOURCE_APPLICATION:	cout << "APPLICATION"; break;
		case GL_DEBUG_SOURCE_OTHER:	cout << "OTHER"; break;
	}

	cout << ", Type: ";
	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR: cout << "ERROR"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	cout << "DEPRECATED_BEHAVIOR"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: cout << "UNDEFINED_BEHAVIOR"; break;
		case GL_DEBUG_TYPE_PORTABILITY:	cout << "PORTABILITY"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:	cout << "PERFORMANCE"; break;
		case GL_DEBUG_TYPE_OTHER: cout << "OTHER"; break;
	}

	cout << ", Severity: ";
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_LOW: cout << "LOW"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: cout << "MEDIUM"; break;
		case GL_DEBUG_SEVERITY_HIGH: cout << "HIGH"; break;
	}
	cout << ", ID: " << id << ")" << endl << endl;
}
// DebugMessageCallback() /////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: initOpenGLDebugCallback()
// purpose:  This function is used to register an OpenGL glDebugMessageCallback function.
//           (Only available in compatibility profile mode and with OpenGL version > 4.2)
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilGLSL::initOpenGLDebugCallback(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	if (glDebugMessageCallback)
	{
		cout << "OpenGL debug message callback successfully registerd." << endl << endl;
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback((GLDEBUGPROC) DebugMessageCallback, NULL);

		GLuint unusedIds = 0;
		glDebugMessageControl(GL_DONT_CARE,	GL_DONT_CARE, GL_DONT_CARE,	0, &unusedIds, true);
	}
	else
		cout << "OpenGL glDebugMessageCallback function not available" << endl << endl;
}
// initOpenGLDebugCallback() //////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: checkOpenGLErrorCode()
// purpose:  This function is called to check for OpenGL errors status flag.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilGLSL::checkOpenGLErrorCode(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		cout << "OpenGL Get Error Code: " << errCode << " (" << errString << ")" << endl << endl;
	}
}
// checkOpenGLErrorCode() /////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: checkShaderInfoLog()
// purpose:  This function is called to check the information log for the
//           given shader object id and print the log if available.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilGLSL::checkShaderInfoLog(GLuint shader)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	int infoLogLength = 0;
	GLchar *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 1)   // infoLog contains at least null char
	{
		infoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
		cout << "Shader InfoLog : " << endl <<  infoLog << endl << endl;
		delete[] infoLog;
	}
	else
	{
		cout << "Shader InfoLog : no errors" << endl << endl;
	}
	checkOpenGLErrorCode();  // check for OpenGL errors
}
// checkShaderInfoLog() ///////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////
// function: checkProgramInfoLog()
// purpose:  This function is called to check the information log for the
//           given program object id and print the log if available.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtilGLSL::checkProgramInfoLog(GLuint program)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	int infoLogLength = 0;
	GLchar *infoLog;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 1)   // infoLog contains at least null char
	{
		infoLog =  new GLchar[infoLogLength];
		glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
		cout << "Program InfoLog: " << endl <<  infoLog << endl << endl;
		delete[] infoLog;
	}
	else
	{
		cout << "Program InfoLog: no errors" << endl << endl;
	}
	checkOpenGLErrorCode();  // check for OpenGL errors
}
// checkProgramInfoLog() //////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////
// function: readShaderFile()
// purpose:  This function is called to read the source code of a vertex or
//           fragment shader. It is the caller's responsibility to destroy
//           the shader source code text buffer using delete[].
///////////////////////////////////////////////////////////////////////////////////////////////////
char* UtilGLSL::readShaderFile(const string& filename)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// local variables
	ifstream shader_file;
	streamoff shader_size = 0;
	char*    shader_code = NULL;

	// set input stream to throw exceptions
	shader_file.exceptions(ios::badbit | ios::failbit | ios::eofbit);

	try // opening shader file
	{
		shader_file.open(filename.c_str(), ios_base::in | ios_base::binary);

		// determine file size
		shader_file.seekg(0, ios_base::end);
		shader_size = shader_file.tellg();
		cout << "Reading Shader : " << filename.c_str()
			<< " (" << shader_size << " bytes)" << endl;

		// read shader source code file
		shader_code = new char[(unsigned int)shader_size + 1];
		shader_file.seekg(0, ios_base::beg);
		shader_file.read(shader_code, shader_size);
		shader_code[shader_size] = '\0';

		// DEBUG: cout << shader_code << endl;
		shader_file.close();
	}
	// exception handlers for file opening
	catch ( exception &e )
	{
		cout << "Error opening file: " << filename.c_str() << " (" << typeid(e).name() << ")" << endl;
		// DEBUG: cout << "Exception class   : " << typeid(e).name()  << endl;
		// DEBUG: cout << "Error description : " << e.what() << endl;
		// local shader_file ifstream object is deleted automatically
	}
	catch (...)
	{
		// handle all other I/O exceptions
		cout << "Unknown error opening file: " << filename.c_str() << endl;
		// local shader_file ifstream object is deleted automatically
	}

	return shader_code;
}
// readShaderFile() ///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: initShaderProgram()
// purpose:  This function is called to compile and link the GLSL shader
//           program with the command line provided vertex and fragment
//           shader code file(s). If successful, the program ID is returned.
///////////////////////////////////////////////////////////////////////////////////////////////////
GLuint UtilGLSL::initShaderProgram(int argc, char **argv)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	GLuint program = 0;
	GLuint shader = 0;

	// delete current program and attached shaders flagged for deletion
	glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&program);
	glDeleteProgram(program);
	program = glCreateProgram();
	// DEBUG: cout << "Program ID = " << program << endl;

	if ( argc == 1)
	{
		// no command line arguments specified, use default rendering
		cout << "Warning: no command line shader specified, using default rendering" << endl;
	}
	else
	{
		// loop through command line specified array of filename strings
		for (int i = 1; i < argc; ++i)
		{
			string filename = argv[i];

			if (filename.find(".vert") != string::npos)
			{
				// create vertex shader object
				shader = glCreateShader(GL_VERTEX_SHADER);
			}
			else if (filename.find(".frag") != string::npos)
			{
				// create fragment shader object
				shader = glCreateShader(GL_FRAGMENT_SHADER);
			}
			else if (filename.find(".geom") != string::npos)
			{
				// create fragment shader object
				shader = glCreateShader(GL_GEOMETRY_SHADER);
			}
			else if (filename.find(".tess") != string::npos)
			{
				// create fragment shader object
				shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			}
			else if (filename.find(".tecs") != string::npos)
			{
				// create fragment shader object
				shader = glCreateShader(GL_TESS_CONTROL_SHADER);
			}
			else
			{
				cout << "Error: Unknown shader file (" << filename << ")" << endl << endl;
				continue;
			}

			// cout << "DEBUG: Shader ID = " << shader << endl;

			const char* shader_code = readShaderFile(filename);
			if (shader_code != NULL)
			{
				glShaderSource(shader, 1, &shader_code, NULL);
				delete[] shader_code;	shader_code = 0;
			}
			else
			{
				cout << "Error: Unable to load shader source code (" << filename << ")" << endl << endl;
			}


			glCompileShader(shader);
			checkShaderInfoLog(shader);
			glAttachShader(program, shader);
			glDeleteShader(shader);          // flag shader for deletion
		}
	}

	cout << "Linking Program: " << endl;
	glLinkProgram(program);
	checkProgramInfoLog(program);

	// check for successful program linking
	GLint successful = false;
	glGetProgramiv(program, GL_LINK_STATUS, &successful);
	if (successful)
	{
		glUseProgram(program);
	}
	else
	{
		cout << "Error: linking shader program, using default rendering" << endl;
		program = 0;
	}

	checkOpenGLErrorCode();
	return program;
}
// initShaderProgram() ////////////////////////////////////////////////////////////////////////////

