#include "objects/object3D.h"
#include "shaderClass.h"
#include "camera.h"       // For Camera class
// #include "light.h"     // Assuming LightProperties is defined
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "physics/AABB.h" // Ensure AABB is included

// If LightProperties is in its own header:
// #include "light.h"
// If not, ensure its definition is visible here.
// For now, assuming LightProperties struct definition is accessible.
struct LightProperties; // Make sure this matches the definition used in the header

void object3D::draw(Shader& shader, Camera& camera, const LightProperties& light, const glm::mat4& parentWorldMatrix) {
    glm::mat4 currentObjectWorldMatrix = parentWorldMatrix * this->localModelMatrix;

    // If object3D itself had its own mesh to render, it would be drawn here.
    // e.g., if (m_mesh) m_mesh->draw(shader, camera, light, currentObjectWorldMatrix);

    for (const auto &child_drawable : drawables_) {
        if (child_drawable) {
            // The currentObjectWorldMatrix of this object3D is the parentWorldMatrix for its children.
            child_drawable->draw(shader, camera, light, currentObjectWorldMatrix);
        }
    }
}

// Calculates the world AABB for this object3D node, including all its children.
Physics::AABB object3D::getWorldAABB(const glm::mat4& parentWorldMatrix) const {
    glm::mat4 currentObjectWorldMatrix = parentWorldMatrix * this->localModelMatrix;
    Physics::AABB combinedAABB = this->localAABB.transform(currentObjectWorldMatrix);

    for (const auto& child_drawable : drawables_) {
        if (child_drawable) {
            Physics::AABB childWorldAABB = child_drawable->getWorldAABB(currentObjectWorldMatrix);
            combinedAABB.expand(childWorldAABB);
        }
    }
    return combinedAABB;
}