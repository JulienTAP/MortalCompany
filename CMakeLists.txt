cmake_minimum_required(VERSION 3.31.6)

project(OpenGLProgram)

# Set directories
set(SRC_DIR src)
set(INCLUDE_DIR include)
set(EXTERNAL_DIR external)
set(GLEW_DIR ${EXTERNAL_DIR}/glew-cmake)
set(GLFW_DIR ${EXTERNAL_DIR}/glfw)
set(GLM_DIR ${EXTERNAL_DIR}/glm)

# Include directories
include_directories(${INCLUDE_DIR})
include_directories(${GLEW_DIR}/include)
include_directories(${GLFW_DIR}/include)
include_directories(${GLM_DIR})

# Source files
set(SOURCES
    ${SRC_DIR}/stb_image.cpp
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/VAO.cpp
    ${SRC_DIR}/VBO.cpp
    ${SRC_DIR}/EBO.cpp
    ${SRC_DIR}/shaderClass.cpp
    ${SRC_DIR}/texture.cpp
    ${SRC_DIR}/camera.cpp
)

# Define Shader director
add_compile_definitions(SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")

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

# Executable
add_executable(opengl_program ${SOURCES})
target_link_libraries(opengl_program ${LIBS})

set_target_properties(opengl_program PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_target_properties(opengl_program PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_target_properties(opengl_program PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ../shaders ${CMAKE_BINARY_DIR}/bin/shaders
    COMMENT "Copying shader files to runtime directory"
)

add_dependencies(opengl_program copy_shaders)

add_custom_target(copy_textures ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ../textures ${CMAKE_BINARY_DIR}/bin/textures
    COMMENT "Copying textures files to runtime directory"
)

add_dependencies(opengl_program copy_textures)