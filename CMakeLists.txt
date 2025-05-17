cmake_minimum_required( VERSION 3.5 )

project( avernus )

IF (WIN32)
  # Statically link standard libraries on Windows. TODO: make compatible with msvc
  set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
ENDIF()

set (OpenGL_GL_PREFERENCE GLVND)

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} ./src ./include )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_WAYLAND OFF)

add_subdirectory( glfw )
add_subdirectory( glm )
add_subdirectory( miniaudio )
add_subdirectory( flecs )

include_directories("${GLFW_SOURCE_DIR}/deps")
set( GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" )

set (AVE-ENGINE
    src/engine/engine.cpp
    src/engine/module.cpp
    src/engine/graphics.cpp
    src/engine/mesh.cpp
    src/engine/shader.cpp
    src/engine/texture.cpp
)

set( AVE-UTILS
    src/utils/fs.cpp
    src/utils/path.cpp
    src/utils/time.cpp
)

set( AVE-EXTERN
    ./include/stb_image.c
    ./include/stb_image_write.c
    ./include/tiny_obj_loader.cc
    ./include/glad/glad.c
)

set (AVE-APP
    src/game/game.cpp
    src/game/input.cpp
)


set( AVE-SRC ${AVE-ENGINE} ${AVE-UTILS} ${AVE-EXTERN} ${AVE-APP} src/main.cpp )

add_executable( avernus ${AVE-SRC} ${GLAD_GL} )

target_link_libraries( avernus ${OPENGL_LIBRARIES} glfw glm miniaudio flecs::flecs_static )
