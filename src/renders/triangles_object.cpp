#include "renders/triangles_object.h"
#include "shaderClass.h"
#include "buffers/vao_buffer.hpp"
#include "buffers/general_buffer.hpp" // Included for potential EBO size check logic
#include <glm/gtc/type_ptr.hpp>
#include <iostream> // For potential warnings/errors

// Constructor is now fully defined in the header (TrianglesObject.h)
// due to the template-like nature of std::unique_ptr and direct AABB calculation.

void TrianglesObject::draw(Shader& shader, const glm::mat4& cameraMatrix, const glm::mat4& parentWorldMatrix) {
    if (!m_vao) {
        throw std::runtime_error("TrianglesObject::draw called with null VAOBuffer");
    }
    // It's okay if m_numIndices is 0, just means nothing visible will be drawn.
    // The check for m_numIndices == 0 and returning might be desired if it's an error state,
    // but for an empty object it's valid.

    // Calculate the final world matrix for this object
    glm::mat4 currentObjectWorldMatrix = parentWorldMatrix * this->localModelMatrix;

    // Activate the shader program
    shader.Activate();

    // Set the model matrix uniform
    GLint modelLoc = glGetUniformLocation(shader.ID, "model");
    if (modelLoc != -1) {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(currentObjectWorldMatrix));
    } else {
        // Optional: std::cerr << "Warning: Uniform 'model' not found in shader." << std::endl;
    }

    // Set the camera (view-projection) matrix uniform
    GLint camMatrixLoc = glGetUniformLocation(shader.ID, "camMatrix");
    if (camMatrixLoc != -1) {
        glUniformMatrix4fv(camMatrixLoc, 1, GL_FALSE, glm::value_ptr(cameraMatrix));
    } else {
        // Optional: std::cerr << "Warning: Uniform 'camMatrix' not found in shader." << std::endl;
    }

    // Bind the VAO
    m_vao->quickBind();

    // Activate necessary vertex attributes (if not persistently enabled in VAO setup)
    // Assuming VAO setup correctly enables attributes.
    // If not, you might need:
    // m_vao->activateSlot(VAOSlot::kPositions);
    // ... and other used slots

    if (m_numIndices > 0) { // Only draw if there are indices
        // The type of indices (GL_UNSIGNED_INT, GL_UNSIGNED_SHORT, etc.)
        // should ideally be known or queried from the EBO.
        // For now, assuming GL_UNSIGNED_INT as it's common.
        GLenum indexType = GL_UNSIGNED_INT;
        // One could potentially store this or deduce it from the EBO's GeneralBuffer's mapping if complex.
        // For example: if (auto ebo = m_vao->getBufferAt(VAOSlot::kIndices).lock()) { indexType = ebo->getDataMapAt(0).type; }

        glDrawElements(GL_TRIANGLES, m_numIndices, indexType, nullptr);
    }

    VAOBuffer::unbind();
}
