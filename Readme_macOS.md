Toolchains
---

To be able to do any meaningful work on macOS with either Objective-C, 
C or C++ you must first install the Apple Development toolchain. 
To do so, log in to the Apple App Store and install Xcode. You will
be guided through the rest of the setup (incl. the command line tools).

Furthermore, you need to install CMake. Download it from here:
https://cmake.org/download/ and install it accordingly. 
Then follow the instructions given in class.

OpenGL
---

Either Core OpenGL up to 4.1 or Legacy OpenGL 2.1 is supported! 
Eliminate everything in the code which uses features > 4.1.

GLEW
---

Not needed on macOS. To be consistent with the other platforms,
the library is there anyway. Therefore, checks are made easier and
no preprocessor hacks are needed.

The header file `<GL/glew.h>` needs to be included before any OpenGL/GLUT
specific headers and `glewExperimental = GL_TRUE;` needs to be set before `glewInit();` .

There might be some `OpenGL Get Error Code: 1280 (invalid enumerant)`
error messages during rendering. This is due to GLEW according to:
https://www.opengl.org/wiki/OpenGL_Loading_Library (GLEW section).

FLTK for macOS
---

FLTK (the version from https://github.com/IngwiePhoenix/FLTK.git) uses either the 
Core Profile or the Legacy Profile (default). To enable the Core Profile, add the 
`FL_OPENGL3` flag to the argument list of `glutCreateWindow(...| FL_OPENGL3);` .  

GLUT
---

Replace all include directives with `<FL/glut.H>` .

Freeglut
---

Not needed on macOS. Replaced with FLTK's GLUT implementation.


This document
--

Converted from Markdown to PDF using the service http://www.markdowntopdf.com

Author and Date: peter.vonniederhaeusern@bfh.ch, 09 September 2016