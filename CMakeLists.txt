cmake_minimum_required(VERSION 3.14)
project(Nature)

#
# CONFIGURATION
#

# Basic CMake settings
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/_install)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data)
set(GLEW_USE_STATIC_LIBS TRUE)

# Use add_resources function to convert .glsl to a .cpp files which will be compiled into a separate static library
include(add_resources)

#
# DEPENDENCIES
#

set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/dependencies/include/")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/dependencies/lib/mingw")

# Warnings for Debug mode
option(USE_STRICT_COMPILE_WARNINGS "Use strict compilation warnings in debug mode." ON)
# These compile flags should apply for GCC and Clang compilers
set(STRICT_COMPILE_FLAGS "-Wpedantic -Wall -Wno-c++98-compat -Wextra -Wno-sign-conversion -Wno-unused-parameter")
if (USE_STRICT_COMPILE_WARNINGS)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${STRICT_COMPILE_FLAGS}")
endif ()

# Find required packages
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(OpenGL REQUIRED)


#
# Shaders / Materials
#

set(PPGSO_SHADER_SRC
        shader/color_vert.glsl shader/color_frag.glsl
        shader/convolution_vert.glsl shader/convolution_frag.glsl
        shader/diffuse_vert.glsl shader/diffuse_frag.glsl
        shader/texture_vert.glsl shader/texture_frag.glsl
        )
add_resources(shaders ${PPGSO_SHADER_SRC})

# PPGSO library
add_library(ppgso STATIC
        ppgso/mesh.cpp
        ppgso/tiny_obj_loader.cpp
        ppgso/shader.cpp
        ppgso/image.cpp
        ppgso/image_bmp.cpp
        ppgso/image_raw.cpp
        ppgso/texture.cpp
        ppgso/window.cpp
        )


# Make sure GLM uses radians and GLEW is a static library
target_compile_definitions(ppgso PUBLIC -DGLM_FORCE_RADIANS -DGLEW_STATIC)

# Link to GLFW, GLEW and OpenGL
target_link_libraries(ppgso PUBLIC ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

# Pass on include directories
target_include_directories(ppgso PUBLIC
        ${GLFW_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR})

#
# TARGETS
#

add_executable(Nature src/main.cpp src/camera.cpp src/object.cpp src/scene.cpp src/camera.h src/object.h src/scene.h src/simpleObject.h src/simpleObject.cpp src/player.h src/player.cpp src/popup.h src/popup.cpp src/rock.h src/rock.cpp src/animator.h src/animator.cpp src/animal.h src/animal.cpp src/cloud.h src/cloud.cpp)
target_link_libraries(Nature ppgso shaders)
install (TARGETS Nature DESTINATION .)

#
# INSTALLATION
#
file(COPY "data/" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(DIRECTORY data/ DESTINATION .)
