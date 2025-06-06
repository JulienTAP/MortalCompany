cmake_minimum_required(VERSION 3.31.6)

project(OpenGLProgram)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Shaders directory
set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders/)
add_compile_definitions(SHADERS_DIR="${SHADERS_DIR}")
# Textures directory
set(TEXTURES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/textures/)
add_compile_definitions(TEXTURES_DIR="${TEXTURES_DIR}")
# Models directory
set(MODELS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/models/)
add_compile_definitions(MODELS_DIR="${MODELS_DIR}")

# Set directories
set(INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(EXTERNAL_DIR external)
set(GLEW_DIR ${EXTERNAL_DIR}/glew-cmake)
set(GLFW_DIR ${EXTERNAL_DIR}/glfw)
set(GLM_DIR ${EXTERNAL_DIR}/glm)

set(ASSIMP_BUILD_TESTS OFF) # Don't build assimp's tests
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF) # Don't build the command-line tools
add_subdirectory(${EXTERNAL_DIR}/assimp)
set(LIBS ${LIBS} assimp)

# Include directories
include_directories(${INCLUDES})
include_directories(${GLEW_DIR}/include)
include_directories(${GLFW_DIR}/include)
include_directories(${GLM_DIR})

# GLEW library
set(GLEW_BUILD_SHARED OFF) # Build static library
set(GLEW_BUILD_STATIC ON)
add_subdirectory(${GLEW_DIR})
set(LIBS ${LIBS} libglew_static)

# GLFW library
add_subdirectory(${GLFW_DIR})
set(LIBS ${LIBS} glfw)

# GLM library
add_subdirectory(${GLM_DIR})

add_compile_options(-Wall -Wextra -Wpedantic -lpthread -lXrandr -lXi -ldl)

add_subdirectory(src)