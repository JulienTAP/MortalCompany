#ifndef TRIANGLES_OBJECT_H
#define TRIANGLES_OBJECT_H

#include <memory>
#include <stdexcept> // Required for std::runtime_error
#include "drawable.h"
#include "buffers/vao_buffer.hpp"
#include "shaderClass.h" // Ensure Shader is declared
#include "glm/glm.hpp"   // Ensure glm::mat4 is declared

class TrianglesObject final : public Drawable { // Inherit from Drawable
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
     * @param numIndices The number of indices to draw.
     */
    explicit TrianglesObject(std::unique_ptr<VAOBuffer> vao, GLsizei numIndices)
        : Drawable(glm::mat4(1.0f)), m_vao(std::move(vao)), m_numIndices(numIndices) { // Initialize Drawable base and localModelMatrix
        if (!m_vao) {
            throw std::runtime_error("VAOBuffer cannot be null for TrianglesObject");
        }
        if (m_numIndices == 0) {
            // Consider warning or error if numIndices is 0, though it might be valid in some edge cases.
            // For now, allow it.
            m_numIndices = m_vao->getBufferAt(VAOSlot::kIndices).lock()->getSize() / sizeof(GLuint); // Calculate number of indices from EBO size
        }
    }
private:
    std::unique_ptr<VAOBuffer> m_vao; /**< The VAOBuffer used for rendering triangles. */
    GLsizei m_numIndices;             /**< Number of indices to draw. */
};

#endif //TRIANGLES_OBJECT_H
