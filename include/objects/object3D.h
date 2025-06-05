#ifndef INC_3DOBJECT_H
#define INC_3DOBJECT_H

#include "buffers/vao_buffer.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <memory>

#include "drawable.h"
#include "shaderClass.h"
#include "physics/AABB.h" // Make sure AABB is included

class object3D : public Drawable {
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

    /**
     * @brief Calculates and returns the AABB of this object and all its children in world space.
     * @param parentWorldMatrix The world transformation matrix of this object's parent.
     * @return The combined AABB in world space.
     */
    [[nodiscard]] Physics::AABB getWorldAABB(const glm::mat4& parentWorldMatrix) const override;


    // Constructor: Initializes Drawable base, which sets up localModelMatrix to identity.
    // localAABB for an object3D container itself is typically default (empty/invalid)
    // unless it also has its own direct geometry (not common for a pure container).
    object3D() : Drawable(glm::mat4(1.0f)) {}

private:
    std::vector<std::unique_ptr<Drawable>> drawables_{};
};

#endif //INC_3DOBJECT_H
