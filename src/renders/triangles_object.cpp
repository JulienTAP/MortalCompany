#include "renders/triangles_object.h"
#include "shaderClass.h"
#include "buffers/vao_buffer.hpp"
#include "buffers/general_buffer.hpp"
#include "camera.h"      // For Camera class
// #include "light.h"    // Assuming LightProperties is defined
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string> // Required for std::string, std::to_string

// If LightProperties is in its own header:
// #include "light.h"
// If not, ensure its definition is visible here.
struct LightProperties; // Make sure this matches the definition used in the header


void TrianglesObject::draw(Shader& shader, Camera& camera, const LightProperties& light, const glm::mat4& parentWorldMatrix) {
    if (!m_vao) {
        throw std::runtime_error("TrianglesObject::draw called with null VAOBuffer");
    }

    glm::mat4 currentObjectWorldMatrix = parentWorldMatrix * this->localModelMatrix;

    shader.Activate();

    // Set model matrix
    GLint modelLoc = glGetUniformLocation(shader.ID, "model");
    if (modelLoc != -1) {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(currentObjectWorldMatrix));
    }

    // Set camera view-projection matrix and camera position
    camera.Matrix(shader, "camMatrix"); // Exports combined view-projection matrix
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

    // Set light uniforms
    glUniform3fv(glGetUniformLocation(shader.ID, "lightPos"), 1, glm::value_ptr(light.position));
    glUniform4fv(glGetUniformLocation(shader.ID, "lightColor"), 1, glm::value_ptr(light.color));

    // Bind textures
    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;
    // Note: The current Texture class takes GLuint slot in constructor.
    // Mesh::Draw dynamically assigns units with i.
    // We should ensure textures are loaded to distinct slots or manage units carefully.
    // For simplicity, matching Mesh::Draw's dynamic unit assignment based on texture type and count:
    for (unsigned int i = 0; i < m_textures.size(); i++) {
        std::string tex_num_str;
        std::string type = m_textures[i].type; // type is const char*

        if (type == "diffuse") {
            tex_num_str = std::to_string(numDiffuse++);
        } else if (type == "specular") {
            tex_num_str = std::to_string(numSpecular++);
        }
        // Assumes shader expects uniforms like "diffuse0", "specular0", etc.
        // The Texture::texUnit method sets the uniform sampler to the texture unit `i`.
        // The Texture constructor already binds to GL_TEXTURE0 + slot, where slot is passed to constructor.
        // If textures are always loaded to unit 0, 1, etc. sequentially, then m_textures[i].unit could be i.
        // The Mesh::Draw uses 'i' as the unit for the shader. Texture::texUnit uses its `unit` argument.

        // The current Texture class stores `unit` which is set during construction based on `slot`.
        // `textures[i].texUnit` activates the shader and sets the uniform.
        // It also uses the `unit` passed to it, not necessarily `textures[i].unit`.
        // Let's use `i` as the texture unit index for the shader uniform, consistent with Mesh::Draw
        m_textures[i].texUnit(shader, (type + tex_num_str).c_str(), i); // Use 'i' as the texture unit
        glActiveTexture(GL_TEXTURE0 + i); // Activate texture unit `i`
        m_textures[i].Bind();             // Bind the texture
    }


    m_vao->quickBind();

    if (m_numIndices > 0) {
        GLenum indexType = GL_UNSIGNED_INT; // Assuming GLuint indices
        // Query EBO for actual index type if necessary and available from VAOBuffer/GeneralBuffer
        auto ebo_weak_ptr = m_vao->getBufferAt(VAOSlot::kIndices);
        if (auto ebo_shared_ptr = ebo_weak_ptr.lock()) {
             const MappedDataBuffer& ebo_map = ebo_shared_ptr->getDataMapAt(0); // Assuming EBO map is at index 0
             if(ebo_map.type == GL_UNSIGNED_SHORT) indexType = GL_UNSIGNED_SHORT;
             else if(ebo_map.type == GL_UNSIGNED_BYTE) indexType = GL_UNSIGNED_BYTE;
             // else defaults to GL_UNSIGNED_INT
        }
        glDrawElements(GL_TRIANGLES, m_numIndices, indexType, nullptr);
    }

    VAOBuffer::unbind(); // Unbind VAO
    // Unbind textures (optional, good practice if other objects might not set all units)
    for (unsigned int i = 0; i < m_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}