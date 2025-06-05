#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "shaderClass.h"

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
    void translate(glm::vec3 translation);

    /**
     * @brief Rotates the drawable object.
     * @param degrees The rotation angle in degrees.
     * @param axis The axis of rotation.
     */
    void rotate(float degrees, glm::vec3 axis);

    /**
     * @brief Scales the drawable object.
     * @param scale The scaling vector.
     */
    void scale(glm::vec3 scaleVec); // Renamed parameter to avoid conflict with method name

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


    explicit Drawable(glm::mat4 modelMatrix = glm::mat4(1.0f))
        : localModelMatrix(modelMatrix) {}

protected:
    glm::mat4 localModelMatrix;
};

#endif //DRAWABLE_H
