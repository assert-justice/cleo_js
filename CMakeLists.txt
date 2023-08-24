cmake_minimum_required( VERSION 3.5 )

project( cleo )

set (OpenGL_GL_PREFERENCE GLVND)

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} ./include )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory( glfw )
add_subdirectory( quickjspp )

# if( MSVC )
#     SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
# endif()

set( CLEO-SRC
     src/cleo.cpp
     src/extern/glad.c
     src/extern/stb_image.c
     src/extern/miniaudio.c
     src/engine/engine.cpp
     src/vm/vm.cpp
     src/window/window.cpp
     src/renderer/renderer.cpp
     )

     
add_executable( cleo ${CLEO-SRC} )
target_link_libraries( cleo ${OPENGL_LIBRARIES} glfw quickjspp )
# target_link_libraries( cleo quickjspp glfw )
# if( MSVC )
#     if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
#         message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLFW-CMake-starter' as StartUp Project in Visual Studio.\n" )
#     else()
#         set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLFW-CMake-starter )
#     endif()
# endif()