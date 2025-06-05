#ifndef TRIANGLES_OBJECT_H
#define TRIANGLES_OBJECT_H

#include <memory>
#include <vector>
#include <stdexcept>
#include "drawable.h"
#include "buffers/vao_buffer.hpp"
#include "shaderClass.h"
#include "glm/glm.hpp"
#include "physics/AABB.h"
#include "../VBO.h"
#include "../texture.h" // For Texture class
#include "../camera.h"    // For Camera class
#include "../light.h"


class TrianglesObject final : public Drawable {
public:
    /**
     * @brief Draws the triangle object.
     * @param shader The shader program to use for drawing.
     * @param camera The camera providing view, projection, and position.
     * @param light The light properties for illumination.
     * @param parentWorldMatrix The world transformation matrix of the parent object.
     */
    void draw(Shader& shader, Camera& camera, const LightProperties& light, const glm::mat4& parentWorldMatrix) override;

    /**
     * @brief Constructor for TrianglesObject.
     * @param vao A unique pointer to the VAOBuffer containing the triangle data.
     * @param vertices A constant reference to a vector of Vertex objects used to build this object.
     * @param textures A constant reference to a vector of Texture objects for this object.
     * @param numIndices The number of indices to draw. If 0, it tries to deduce from EBO.
     * @param modelMatrix Initial local model matrix.
     */
    explicit TrianglesObject(std::unique_ptr<VAOBuffer> vao,
                             const std::vector<Vertex>& vertices,
                             const std::vector<Texture>& textures, // Added textures parameter
                             GLsizei numIndices = 0,
                             const glm::mat4& modelMatrix = glm::mat4(1.0f))
        : Drawable(modelMatrix), m_vao(std::move(vao)), m_textures(textures), m_numIndices(numIndices) {
        if (!m_vao) {
            throw std::runtime_error("VAOBuffer cannot be null for TrianglesObject");
        }
        // Attempt to get numIndices from EBO if not provided or zero.
        if (m_numIndices == 0) {
            auto ebo_weak_ptr = m_vao->getBufferAt(VAOSlot::kIndices);
            if (auto ebo_shared_ptr = ebo_weak_ptr.lock()) {
                if (ebo_shared_ptr->getType() == GL_ELEMENT_ARRAY_BUFFER && ebo_shared_ptr->getSize() > 0) {
                    const MappedDataBuffer& ebo_map = ebo_shared_ptr->getDataMapAt(0);
                    GLsizei index_size_bytes = 0;
                    if (ebo_map.type == GL_UNSIGNED_INT) index_size_bytes = sizeof(GLuint);
                    else if (ebo_map.type == GL_UNSIGNED_SHORT) index_size_bytes = sizeof(GLushort);
                    else if (ebo_map.type == GL_UNSIGNED_BYTE) index_size_bytes = sizeof(GLubyte);

                    if (index_size_bytes > 0 && ebo_shared_ptr->getSize() % index_size_bytes == 0) {
                         m_numIndices = ebo_shared_ptr->getSize() / index_size_bytes;
                    }
                }
            }
        }
        // Calculate and set the local AABB using the provided vertices.
        setLocalAABB(Physics::AABB::createFromVertices(vertices));
    }

private:
    std::unique_ptr<VAOBuffer> m_vao;
    std::vector<Texture> m_textures; // Store textures
    GLsizei m_numIndices;
};

#endif //TRIANGLES_OBJECT_H