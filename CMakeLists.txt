cmake_minimum_required( VERSION 3.5 )

project( cleo )

set (OpenGL_GL_PREFERENCE GLVND)

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} ./include quickjs ./src)

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory( glfw )
add_subdirectory( quickjs )
add_subdirectory( glm )
add_subdirectory( freetype )

set( CLEO-SRC
     # main
     src/cleo.cpp
     # external
     src/extern/glad.c
     src/extern/stb_image.c
     src/extern/miniaudio.c
     # utils
     src/utils/fs.cpp
     src/utils/cli.cpp
     # engine and subsystems
     src/engine/engine.cpp
     src/vm/vm.cpp
     src/vm/fn_help.cpp
     src/vm/object_help.cpp
     src/window/window.cpp
     src/renderer/renderer.cpp
     src/renderer/shader.cpp
     src/renderer/texture.cpp
     src/audio/audio.cpp
     # bindings
     src/vm/bind_engine.cpp
     src/vm/bind_graphics.cpp
     src/vm/bind_system.cpp
     src/vm/bind_input.cpp
     src/vm/bind_window.cpp
     src/vm/bind_audio.cpp
     )

     
add_executable( cleo ${CLEO-SRC} )
target_link_libraries( cleo ${OPENGL_LIBRARIES} glfw quickjs glm freetype)