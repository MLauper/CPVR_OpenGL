# Locate FLTK
#
# This module defines
# FLTK_LIBRARIES - Link to this, by default it includes all release libs
# FLTK_LIBRARIES_DEBUG - Link to this, by default it includes all debug libs
# FLTK_FOUND, if false, do not try to link to FLTK
# FLTK_INCLUDE_DIRS, where to find the headers
#
#  This module accepts the following variables
#
#  FLTK_ROOT_DIR - Can be set to FLTK install path or Windows build path
#
# Created by Robert Hauck / Urs Kuenzler 

if(WIN32)
    #message("FLTK using win libs")
    set(FLTK_LIBRARIES_PATH "${FLTK_ROOT_DIR}/lib/win")

    set(FLTK_LIBRARY "${FLTK_LIBRARIES_PATH}/Release/fltk.lib") 
    set(FLTK_GL_LIBRARY "${FLTK_LIBRARIES_PATH}/Release/fltk_gl.lib")
    
    set(FLTK_LIBRARY_DEBUG "${FLTK_LIBRARIES_PATH}/Debug/fltkd.lib") 
    set(FLTK_GL_LIBRARY_DEBUG "${FLTK_LIBRARIES_PATH}/Debug/fltk_gld.lib")
 endif()

if(UNIX)
    #message("FLTK using linux libs")
    set(ARCH_LIBRARY_PATH "${FLTK_ROOT_DIR}/lib/linux")

    find_library(FLTK_LIBRARY          NAMES fltk             PATHS "${ARCH_LIBRARY_PATH}/Release" ${CMAKE_LIBRARY_PATH} PATH_SUFFIXES lib)
    find_library(FLTK_GL_LIBRARY       NAMES fltk_gl          PATHS "${ARCH_LIBRARY_PATH}/Release" ${CMAKE_LIBRARY_PATH} PATH_SUFFIXES lib)
    
    find_library(FLTK_LIBRARY_DEBUG    NAMES fltk       PATHS "${ARCH_LIBRARY_PATH}/Debug"   ${CMAKE_LIBRARY_PATH} PATH_SUFFIXES lib)
    find_library(FLTK_GL_LIBRARY_DEBUG NAMES fltk_gl PATHS "${ARCH_LIBRARY_PATH}/Debug"   ${CMAKE_LIBRARY_PATH} PATH_SUFFIXES lib)
endif()

if(APPLE)
   #message("FLTK using mac libs")
   set(FLTK_ROOT_DIR "${CMAKE_SOURCE_DIR}/_LIBS/FLTK")
   set(FLTK_INCLUDE_DIR "${FLTK_ROOT_DIR}/include")

   set(FLTK_LIBRARIES_PATH "${FLTK_ROOT_DIR}/lib/mac")

   set(FLTK_LIBRARY "${FLTK_LIBRARIES_PATH}/Release/libfltk.a") 
   set(FLTK_GL_LIBRARY "${FLTK_LIBRARIES_PATH}/Release/libfltk_gl.a")

   set(FLTK_LIBRARY_DEBUG "${FLTK_LIBRARIES_PATH}/Debug/libfltk.a") 
   set(FLTK_GL_LIBRARY_DEBUG "${FLTK_LIBRARIES_PATH}/Debug/libfltk_gl.a")
endif()

# locate header files and put user specified location at beginning of search
if(FLTK_ROOT_DIR)
    set(_FLTK_HEADER_SEARCH_DIRS "${FLTK_ROOT_DIR}"
                                "${FLTK_ROOT_DIR}/include"
                                 ${_FLTK_HEADER_SEARCH_DIRS})
endif(FLTK_ROOT_DIR)

find_path(FLTK_INCLUDE_DIR "FL/glut.H" PATHS ${_FLTK_HEADER_SEARCH_DIRS})

# handle the QUIETLY and REQUIRED arguments and set FLTK_FOUND to TRUE if all listed variables are TRUE
include("FindPackageHandleStandardArgs")
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FLTK DEFAULT_MSG FLTK_LIBRARY FLTK_LIBRARY_DEBUG FLTK_INCLUDE_DIR)

if(FLTK_FOUND)
   set(FLTK_INCLUDE_DIRS ${FLTK_INCLUDE_DIR})
   set(FLTK_LIBRARIES ${FLTK_LIBRARY} ${FLTK_GL_LIBRARY})
   set(FLTK_LIBRARIES_DEBUG ${FLTK_LIBRARY_DEBUG} ${FLTK_GL_LIBRARY_DEBUG})
endif(FLTK_FOUND)

