#ifndef TRIANGLES_OBJECT_H
#define TRIANGLES_OBJECT_H

#include <memory>
#include <vector> // Required for std::vector
#include <stdexcept> // Required for std::runtime_error
#include "drawable.h"
#include "buffers/vao_buffer.hpp"
#include "shaderClass.h"
#include "glm/glm.hpp"
#include "physics/AABB.h" // Include AABB header
#include "../VBO.h"       // Include Vertex struct definition (adjust path if necessary)


class TrianglesObject final : public Drawable {
public:
    /**
     * @brief Draws the triangle object.
     * @param shader The shader program to use for drawing.
     * @param cameraMatrix The view-projection matrix from the camera.
     * @param parentWorldMatrix The world transformation matrix of the parent object.
     */
    void draw(Shader& shader, const glm::mat4& cameraMatrix, const glm::mat4& parentWorldMatrix) override;

    /**
     * @brief Constructor for TrianglesObject.
     * @param vao A unique pointer to the VAOBuffer containing the triangle data.
     * @param vertices A constant reference to a vector of Vertex objects used to build this object.
     * This data is used to calculate the local AABB.
     * @param numIndices The number of indices to draw.
     * @param modelMatrix Initial local model matrix.
     */
    explicit TrianglesObject(std::unique_ptr<VAOBuffer> vao,
                             const std::vector<Vertex>& vertices, // Added vertices parameter
                             GLsizei numIndices,
                             const glm::mat4& modelMatrix = glm::mat4(1.0f))
        : Drawable(modelMatrix), m_vao(std::move(vao)), m_numIndices(numIndices) {
        if (!m_vao) {
            throw std::runtime_error("VAOBuffer cannot be null for TrianglesObject");
        }
        if (m_numIndices == 0) {
            // Attempt to get numIndices from EBO if not provided or zero.
            // This assumes the EBO is always bound to VAOSlot::kIndices and contains GLuints.
            auto ebo_weak_ptr = m_vao->getBufferAt(VAOSlot::kIndices);
            if (auto ebo_shared_ptr = ebo_weak_ptr.lock()) {
                if (ebo_shared_ptr->getType() == GL_ELEMENT_ARRAY_BUFFER && ebo_shared_ptr->getSize() > 0) {
                    // Assuming EBO stores GLuint indices.
                    // The mapping for EBO in GeneralBuffer should specify GL_UNSIGNED_INT for its type.
                    // Example: ebo_source.mapping = {{GL_UNSIGNED_INT, ComponentSize::kOne}};
                    // And its dataMap (m_datatypes in GeneralBuffer) should reflect this.
                    // For simplicity, we'll assume the first data map entry of the EBO describes the index type.
                    const MappedDataBuffer& ebo_map = ebo_shared_ptr->getDataMapAt(0);
                    GLsizei index_size_bytes = 0;
                    if (ebo_map.type == GL_UNSIGNED_INT) index_size_bytes = sizeof(GLuint);
                    else if (ebo_map.type == GL_UNSIGNED_SHORT) index_size_bytes = sizeof(GLushort);
                    else if (ebo_map.type == GL_UNSIGNED_BYTE) index_size_bytes = sizeof(GLubyte);

                    if (index_size_bytes > 0 && ebo_shared_ptr->getSize() % index_size_bytes == 0) {
                         m_numIndices = ebo_shared_ptr->getSize() / index_size_bytes;
                    } else {
                        // Warning or throw: Could not determine index size or EBO size is not multiple of index size
                    }
                }
            }
            if (m_numIndices == 0) {
                 // std::cerr << "Warning: TrianglesObject created with 0 indices and could not deduce from EBO." << std::endl;
            }
        }

        // Calculate and set the local AABB using the provided vertices.
        // The AABB is created in local space (identity transform for vertices).
        setLocalAABB(Physics::AABB::createFromVertices(vertices));
    }

private:
    std::unique_ptr<VAOBuffer> m_vao; /**< The VAOBuffer used for rendering triangles. */
    GLsizei m_numIndices;             /**< Number of indices to draw. */
    // std::vector<Vertex> m_vertices; // Optionally store vertices if needed later, but not strictly for AABB calculation here
};

#endif //TRIANGLES_OBJECT_H
