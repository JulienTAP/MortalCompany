#ifndef INC_3DOBJECT_H
#define INC_3DOBJECT_H

#include "buffers/vao_buffer.hpp" // Though not directly used by object3D itself for drawing
#include "glm/fwd.hpp"
#include "glm/glm.hpp" // For glm::mat4
#include <vector>
#include <memory> // For std::unique_ptr

#include "drawable.h" // Base class
#include "shaderClass.h" // For Shader&

class object3D : public Drawable { // object3D is also a Drawable
public:
    /**
     * @brief Draws the object and its children.
     * @param shader The shader program to use for drawing.
     * @param cameraMatrix The view-projection matrix from the camera.
     * @param parentWorldMatrix The world transformation matrix of this object's parent.
     */
    void draw(Shader& shader, const glm::mat4& cameraMatrix, const glm::mat4& parentWorldMatrix) override;

    /**
     * @brief Adds a drawable child object.
     * @param drawable A unique_ptr to the Drawable object to add.
     */
    void addObject(std::unique_ptr<Drawable> drawable) {
        if (drawable) {
            drawables_.emplace_back(std::move(drawable));
        }
    }

    /**
     * @brief Gets the count of drawable children.
     * @return The number of child drawables.
     */
    [[nodiscard]] size_t getDrawablesCount() const {
        return drawables_.size();
    }

    /**
     * @brief Gets a specific drawable child.
     * @param index The index of the child.
     * @return A raw pointer to the child Drawable, or nullptr if index is out of bounds.
     */
    [[nodiscard]] Drawable* getDrawable(size_t index) const {
        if (index < drawables_.size()) {
            return drawables_.at(index).get();
        }
        return nullptr;
    }

    // Constructor: Initializes Drawable base, which sets up localModelMatrix to identity.
    object3D() : Drawable(glm::mat4(1.0f)) {}

private:
    std::vector<std::unique_ptr<Drawable>> drawables_{};
};

#endif //INC_3DOBJECT_H
