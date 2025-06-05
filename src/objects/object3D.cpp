#include "objects/object3D.h"
#include "shaderClass.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "physics/AABB.h" // Ensure AABB is included

void object3D::draw(Shader& shader, const glm::mat4& cameraMatrix, const glm::mat4& parentWorldMatrix) {
    glm::mat4 currentObjectWorldMatrix = parentWorldMatrix * this->localModelMatrix;

    // If object3D itself had a mesh to render (e.g., if it wasn't just a container),
    // it would be drawn here using currentObjectWorldMatrix.
    // For example:
    // if (m_trianglesObject) { // Assuming object3D could have its own TrianglesObject
    //     m_trianglesObject->draw(shader, cameraMatrix, currentObjectWorldMatrix);
    // }

    for (const auto &child_drawable : drawables_) {
        if (child_drawable) {
            child_drawable->draw(shader, cameraMatrix, currentObjectWorldMatrix);
        }
    }
}

// Calculates the world AABB for this object3D node, including all its children.
Physics::AABB object3D::getWorldAABB(const glm::mat4& parentWorldMatrix) const {
    // Calculate the world matrix for this object3D node itself.
    glm::mat4 currentObjectWorldMatrix = parentWorldMatrix * this->localModelMatrix;

    // Start with this object's own localAABB transformed to world space.
    // If object3D is purely a container, its localAABB might be "invalid" (min > max),
    // which AABB::expand should handle correctly when combining with children.
    Physics::AABB combinedAABB = this->localAABB.transform(currentObjectWorldMatrix);

    // Iterate through children and expand the AABB.
    for (const auto& child_drawable : drawables_) {
        if (child_drawable) {
            // The 'currentObjectWorldMatrix' of this object3D is the 'parentWorldMatrix' for its children.
            Physics::AABB childWorldAABB = child_drawable->getWorldAABB(currentObjectWorldMatrix);
            combinedAABB.expand(childWorldAABB);
        }
    }
    return combinedAABB;
}
