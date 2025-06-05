#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "shaderClass.h"
#include "physics/AABB.h" // Include the AABB header

class Drawable {
public:
    virtual ~Drawable() = default;

    /**
     * @brief Draws the drawable object.
     * @param shader The shader program to use for drawing.
     * @param cameraMatrix The view-projection matrix from the camera.
     * @param parentWorldMatrix The world transformation matrix of the parent object.
     */
    virtual void draw(Shader& shader, const glm::mat4& cameraMatrix, const glm::mat4& parentWorldMatrix) = 0;

    /**
     * @brief Translates the drawable object.
     * @param translation The translation vector.
     */
    void translate(const glm::vec3& translation); // Added const&

    /**
     * @brief Rotates the drawable object.
     * @param degrees The rotation angle in degrees.
     * @param axis The axis of rotation.
     */
    void rotate(float degrees, const glm::vec3& axis); // Added const&

    /**
     * @brief Scales the drawable object.
     * @param scaleVec The scaling vector.
     */
    void scale(const glm::vec3& scaleVec); // Added const&

    /**
     * @brief Gets the local model matrix of the drawable.
     * @return The local model matrix.
     */
    [[nodiscard]] const glm::mat4& getLocalModelMatrix() const { return localModelMatrix; }

    /**
     * @brief Sets the local model matrix of the drawable.
     * @param matrix The new local model matrix.
     */
    void setLocalModelMatrix(const glm::mat4& matrix) { localModelMatrix = matrix; }

    /**
     * @brief Gets the AABB of this drawable in world space.
     * @param parentWorldMatrix The world transformation matrix of the parent object.
     * @return The AABB in world space.
     */
    [[nodiscard]] virtual Physics::AABB getWorldAABB(const glm::mat4& parentWorldMatrix) const {
        // Default implementation for objects that might not have their own vertices (like object3D root)
        // Derived classes with actual geometry (like TrianglesObject) should override this
        // or ensure localAABB is meaningful.
        // For a simple Drawable that isn't an object3D container, this transform is correct.
        // For an object3D container, it should combine children's AABBs.
        glm::mat4 worldMatrix = parentWorldMatrix * localModelMatrix;
        return localAABB.transform(worldMatrix);
    }

    /**
     * @brief Gets the local AABB of this drawable.
     * @return The local AABB.
     */
    [[nodiscard]] const Physics::AABB& getLocalAABB() const { return localAABB; }

    /**
     * @brief Sets whether this object is collidable.
     * @param isCollidable True if the object should be considered for collisions.
     */
    void setCollidable(bool isCollidable) { collidable = isCollidable; }

    /**
     * @brief Checks if this object is collidable.
     * @return True if the object is collidable.
     */
    [[nodiscard]] bool isCollidable() const { return collidable; }


    explicit Drawable(glm::mat4 modelMatrix = glm::mat4(1.0f))
        : localModelMatrix(modelMatrix), localAABB(), collidable(false) {} // Initialize localAABB and collidable

protected:
    glm::mat4 localModelMatrix;
    Physics::AABB localAABB; // Local space AABB for this drawable
    bool collidable;         // Flag to indicate if this object participates in collision detection

    /**
     * @brief Sets the local AABB. Primarily for use by derived classes.
     * @param aabb The local AABB.
     */
    void setLocalAABB(const Physics::AABB& aabb) { localAABB = aabb; }
};

#endif //DRAWABLE_H
