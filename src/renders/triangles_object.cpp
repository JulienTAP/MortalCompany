#include "renders/triangles_object.h"
#include "shaderClass.h" // For Shader class
#include "buffers/vao_buffer.hpp" // For VAOBuffer
#include <glm/gtc/type_ptr.hpp> // For glm::value_ptr

void TrianglesObject::draw(Shader& shader, const glm::mat4& cameraMatrix, const glm::mat4& parentWorldMatrix) {
    if (!m_vao) {
        // This should ideally be caught by constructor, but as a safeguard:
        throw std::runtime_error("TrianglesObject::draw called with null VAOBuffer");
    }
    if (m_numIndices == 0) {
        return; // Nothing to draw
    }

    // Calculate the final world matrix for this object
    glm::mat4 worldMatrix = parentWorldMatrix * this->localModelMatrix;

    // Activate the shader program
    shader.Activate();

    // Set the model matrix uniform
    // Ensure your Shader class has a way to get shader.ID or use a helper like shader.setMat4("model", worldMatrix);
    GLint modelLoc = glGetUniformLocation(shader.ID, "model");
    if (modelLoc != -1) {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(worldMatrix));
    } else {
        // std::cerr << "Warning: Uniform 'model' not found in shader." << std::endl;
    }

    // Set the camera (view-projection) matrix uniform
    GLint camMatrixLoc = glGetUniformLocation(shader.ID, "camMatrix");
    if (camMatrixLoc != -1) {
        glUniformMatrix4fv(camMatrixLoc, 1, GL_FALSE, glm::value_ptr(cameraMatrix));
    } else {
        // std::cerr << "Warning: Uniform 'camMatrix' not found in shader." << std::endl;
    }

    // Bind the VAO
    m_vao->quickBind();

    // Draw the elements
    // The count of indices is now stored in m_numIndices.
    // The type of indices is assumed to be GL_UNSIGNED_INT based on typical EBO setup.
    // The last parameter is an offset into the EBO, typically 0 (nullptr).
    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);

    // Unbind the VAO - it's good practice to unbind after drawing,
    // though some rendering strategies might do this at a higher level (e.g., end of render pass).
    VAOBuffer::unbind();

    // Do not deactivate shader here, it might be used by other objects.
}
