set(APP "OpenGL")
set(CWD ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${APP}
        ../include/objects/object3D.h)

SET(
        BUFFERS_DIR
        ${CWD}/buffers/general_buffer.cpp
        ${CWD}/buffers/vao_buffer.cpp
)

SET(
        SRC_DIR
        ${BUFFERS_DIR}
        ${CWD}/drawable.cpp
        ${CWD}/renders/triangles_object.cpp
        ${CWD}/objects/object3D.cpp
        ${CWD}/main.cpp
        ${CWD}/camera.cpp
        ${CWD}/mesh.cpp
        ${CWD}/EBO.cpp
        ${CWD}/VAO.cpp
        ${CWD}/VBO.cpp
        ${CWD}/texture.cpp
        ${CWD}/shaderClass.cpp
        ${CWD}/stb_image.cpp
        ${CWD}/mesh.cpp
        ${CWD}/node.cpp
        ${CWD}/sphere.cpp
        ${CWD}/shape.cpp
        ${CWD}/cylinder.cpp
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