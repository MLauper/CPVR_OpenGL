# CMake file for Hello GLSL demo

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# the 'project' macro is used to name a project
project(CG-03-A.01_3DHouse)



# adjust some global CMake configuration settings
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_COLOR_MAKEFILE TRUE)
set(CMAKE_SUPPRESS_REGENERATION TRUE)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_PREFIX_PATH "${CMAKE_HOME_DIRECTORY}/_LIBS;${CMAKE_PREFIX_PATH}")


# setup package root directories
set(GLEW_ROOT_DIR "${CMAKE_HOME_DIRECTORY}/_LIBS/GLEW")
set(GLM_ROOT_DIR "${CMAKE_HOME_DIRECTORY}/_LIBS/GLM")
set(FLTK_ROOT_DIR "${CMAKE_HOME_DIRECTORY}/_LIBS/FLTK")
if(WIN32)
   if(MSVC)
      add_definitions(-DGLEW_STATIC)
      set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO ${CMAKE_EXE_LINKER_FLAGS}")
      set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG /NODEFAULTLIB:MSVCRT ${CMAKE_EXE_LINKER_FLAGS}")
   endif(MSVC)      
endif(WIN32)


if(CMAKE_CONFIGURATION_TYPES)
   set(CMAKE_INSTALL_PREFIX ./install)
   set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE STRING
     "Reset the configurations to what we need"
     FORCE)
endif()

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES Debug Release)
    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
      "Reset the configurations to what we need"
      FORCE)
endif()


# configure the Visual Studio user file
if(WIN32)
   if(MSVC)
	# find user and system name
	set(VC_USER_SYSTEM_NAME $ENV{USERDOMAIN} CACHE STRING SystemName)
	set(VC_USER_USER_NAME $ENV{USERNAME} CACHE STRING UserName)

	# configure the template file
	set(USER_FILE ${PROJECT_NAME}.vcxproj.user)
	set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})

	# setup working directories in template file
	set(USERFILE_WORKING_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug)
	set(USERFILE_WORKING_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release)
	set(USERFILE_ARGUMENTS_RELEASE "")
	set(USERFILE_ARGUMENTS_DEBUG "")
	configure_file(${CMAKE_HOME_DIRECTORY}/_CMAKE/CG-PROJECTS.vcxproj.usertemplate ${OUTPUT_PATH} @ONLY)
   endif(MSVC)
endif(WIN32)


# check for Linux and make output path adjustments
if ("${CMAKE_SYSTEM}" MATCHES "Linux.*")
  #Set the binary output path to correspond to windows defaults
  set(EXECUTABLE_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}")
endif()


# find sources
file(GLOB SRCS 
    ../_COMMON/src/*.c
    ../_COMMON/src/*.cpp
    ./src/*.c
    ./src/*.cpp
)
source_group("src" FILES ${SRCS})


# find headers (let them show up in the IDEs)
file(GLOB HDRS 
    ../_COMMON/inc/*.h
    ../_COMMON/inc/*.hpp
    ./inc/*.h
    ./inc/*.hpp
)
source_group("inc" FILES ${HDRS})


# find GLSL shaders files (let them show up in the IDEs)
file(GLOB GLSL
    ./glsl/*.frag
    ./glsl/*.vert
    ./glsl/*.geom
    ./glsl/*.tess
    ./glsl/*.tecs
)
source_group("glsl" FILES ${GLSL})


# find packages and libs
find_package(OpenGL REQUIRED)
find_package(FLTK REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)


# find framework (specific to mac)
if (APPLE)
	find_library(COCOA_LIBRARY Cocoa)
elseif()
	set(COCOA_LIBRARY " ")
endif()


# setup package headers
include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${FLTK_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIRS}
)


# setup debug/release libraries
set(LIBS_DEBUG)
foreach(lib; ${OPENGL_LIBRARIES}
             ${FLTK_LIBRARIES_DEBUG}
             ${GLEW_LIBRARIES_DEBUG})
    list(APPEND LIBS_DEBUG debug ${lib})
endforeach()

set(LIBS_RELEASE)
foreach(lib; ${OPENGL_LIBRARIES}
             ${FLTK_LIBRARIES}
             ${GLEW_LIBRARIES})
    list(APPEND LIBS_RELEASE optimized ${lib})
endforeach()


# force older GLM versions to use radians instead of degrees
add_definitions(-DGLM_FORCE_RADIANS)


# define target dependencies and build instructions
set(EXECUTABLE_NAME ${PROJECT_NAME})
add_executable(${EXECUTABLE_NAME} ${SRCS} ${HDRS} ${GLSL})
# add framework (specific to mac)
target_link_libraries(${EXECUTABLE_NAME} ${LIBS_RELEASE} ${LIBS_DEBUG} ${COCOA_LIBRARY})

