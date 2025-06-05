#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "shaderClass.h"
#include "physics/AABB.h" // Include the AABB header
#include "camera.h"       // For Camera class
// #include "light.h"     // Assuming LightProperties is defined (e.g. in light.h)
// For now, let's assume LightProperties is available or we pass params directly.
// For simplicity in this step, we'll pass light params directly to avoid creating a new file now.
// If you create light.h with LightProperties, include it and use const LightProperties& light.

// Forward declare LightProperties if not including a header for it yet
struct LightProperties; // Add this if light.h is not created yet for this step

class Drawable {
public:
    virtual ~Drawable() = default;

    /**
     * @brief Draws the drawable object.
     * @param shader The shader program to use for drawing.
     * @param camera The camera providing view, projection, and position.
     * @param light The light properties for illumination.
     * @param parentWorldMatrix The world transformation matrix of the parent object.
     */
    virtual void draw(Shader& shader, Camera& camera, const LightProperties& light, const glm::mat4& parentWorldMatrix) = 0;

    /**
     * @brief Translates the drawable object.
     * @param translation The translation vector.
     */
    void translate(const glm::vec3& translation);

    /**
     * @brief Rotates the drawable object.
     * @param degrees The rotation angle in degrees.
     * @param axis The axis of rotation.
     */
    void rotate(float degrees, const glm::vec3& axis);

    /**
     * @brief Scales the drawable object.
     * @param scaleVec The scaling vector.
     */
    void scale(const glm::vec3& scaleVec);

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
    void setCollidable(bool isCollidableParam) { collidable = isCollidableParam; }

    /**
     * @brief Checks if this object is collidable.
     * @return True if the object is collidable.
     */
    [[nodiscard]] bool isCollidable() const { return collidable; }


    explicit Drawable(glm::mat4 modelMatrix = glm::mat4(1.0f))
        : localModelMatrix(modelMatrix), localAABB(), collidable(true) {} // Initialize localAABB, default collidable to true

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