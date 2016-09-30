# Locate GLEW
#
# This module defines
# GLEW_LIBRARIES - Link to this, by default it includes all release libs
# GLEW_LIBRARIES_DEBUG - Link to this, by default it includes all debug libs
# GLEW_FOUND, if false, do not try to link to GLEW
# GLEW_INCLUDE_DIRS, where to find the headers
#
#  This module accepts the following variables
#
#  GLEW_ROOT_DIR - Can be set to GLEW install path or Windows build path
#
# Created by Robert Hauck / Urs Kuenzler 

if(WIN32)
    #message("GLEW using win libs")
    set(GLEW_LIBRARIES_PATH "${GLEW_ROOT_DIR}/lib/win")

    set(GLEW_LIBRARY "${GLEW_LIBRARIES_PATH}/Release/libglew32.lib") 
    set(GLEW_LIBRARY_DEBUG "${GLEW_LIBRARIES_PATH}/Debug/libglew32d.lib")
endif()

if(UNIX)
    #message("GLEW using linux libs")
    set(GLEW_LIBRARIES_PATH "${GLEW_ROOT_DIR}/lib/linux")

    set(GLEW_LIBRARY "${GLEW_LIBRARIES_PATH}/Release/libGLEW.a") 
    set(GLEW_LIBRARY_DEBUG "${GLEW_LIBRARIES_PATH}/Debug/libGLEWd.a")
endif()


# locate header files and put user specified location at beginning of search
if(GLEW_ROOT_DIR)
    set(_GLEW_HEADER_SEARCH_DIRS "${GLEW_ROOT_DIR}"
                                "${GLEW_ROOT_DIR}/include"
                                 ${_GLEW_HEADER_SEARCH_DIRS})
endif(GLEW_ROOT_DIR)

find_path(GLEW_INCLUDE_DIR "GL/glew.h" PATHS ${_GLEW_HEADER_SEARCH_DIRS})

if(APPLE)
   set(GLEW_ROOT_DIR "${CMAKE_SOURCE_DIR}/_LIBS/GLEW")
   set(GLEW_INCLUDE_DIR "${GLEW_ROOT_DIR}/include")

   set(GLEW_LIBRARIES_PATH "${GLEW_ROOT_DIR}/lib/mac")
      
   set(GLEW_LIBRARY "${GLEW_LIBRARIES_PATH}/Release/libGLEW.a")
   set(GLEW_LIBRARY_DEBUG "${GLEW_LIBRARIES_PATH}/Debug/libGLEW.a")
endif()

# handle the QUIETLY and REQUIRED arguments and set GLEW_FOUND to TRUE if all listed variables are TRUE
include("FindPackageHandleStandardArgs")
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG GLEW_LIBRARY GLEW_LIBRARY_DEBUG GLEW_INCLUDE_DIR)

if(GLEW_FOUND)
   set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
   set(GLEW_LIBRARIES ${GLEW_LIBRARY})
   set(GLEW_LIBRARIES_DEBUG ${GLEW_LIBRARY_DEBUG})
endif(GLEW_FOUND)

