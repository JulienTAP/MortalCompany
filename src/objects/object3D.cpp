#include "objects/object3D.h"
#include "shaderClass.h" // For Shader
#include "glm/gtc/matrix_transform.hpp" // For matrix operations
#include "glm/gtc/type_ptr.hpp"         // For glm::value_ptr

// The draw method for object3D applies its own local transformation
// to the parent's world transformation, and then passes this new
// transform to its children as their parentWorldMatrix.
void object3D::draw(Shader& shader, const glm::mat4& cameraMatrix, const glm::mat4& parentWorldMatrix) {
    // Calculate the world matrix for this object3D node itself.
    // This is its parent's world matrix multiplied by its own local matrix.
    glm::mat4 currentObjectWorldMatrix = parentWorldMatrix * this->localModelMatrix;

    // object3D itself might not have direct geometry to render,
    // but if it did, it would use 'currentObjectWorldMatrix' as its model matrix.
    // For example:
    // if (this->hasOwnMesh) {
    //     shader.Activate();
    //     glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(currentObjectWorldMatrix));
    //     glUniformMatrix4fv(glGetUniformLocation(shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
    //     // Bind this object's VAO and draw
    // }

    // Draw all child drawables.
    // The 'currentObjectWorldMatrix' becomes the 'parentWorldMatrix' for the children.
    for (const auto &child_drawable : drawables_) {
        if (child_drawable) {
            child_drawable->draw(shader, cameraMatrix, currentObjectWorldMatrix);
        }
    }
    // Note: object3D does not call VAOBuffer::unbind() itself, as it doesn't directly manage a VAO for its own rendering.
    // Its children (like TrianglesObject) are responsible for their VAO binding/unbinding if they render something.
}
