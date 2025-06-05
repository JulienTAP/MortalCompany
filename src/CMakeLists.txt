set(APP "OpenGL")
set(CWD ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${APP})

SET(
        SRC_DIR
        ${CWD}/main.cpp
        ${CWD}/camera.cpp
        ${CWD}/EBO.cpp
        ${CWD}/VAO.cpp
        ${CWD}/VBO.cpp
        ${CWD}/texture.cpp
        ${CWD}/shaderClass.cpp
        ${CWD}/stb_image.cpp
        ${CWD}/mesh.cpp
        ${CWD}/node.cpp
)

target_sources(${APP} PRIVATE ${SRC_DIR})
target_link_libraries(${APP} ${LIBS})

set_target_properties(${APP} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_target_properties(${APP} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_target_properties(${APP} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_custom_target(copy_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADERS_DIR} ${CMAKE_BINARY_DIR}/bin/shaders
        COMMENT "Copying shader files to runtime directory"
)

add_dependencies(${APP} copy_shaders)