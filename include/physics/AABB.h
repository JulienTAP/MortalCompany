#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>
#include <vector>
#include <limits> // Required for std::numeric_limits
#include <algorithm> // Required for std::min and std::max

// Forward declare Vertex struct if its definition is in a separate header (e.g., VBO.h)
// This avoids a circular dependency if AABB.h were included by VBO.h for some reason.
// However, for createFromVertices, we'll need the actual definition.
// Assuming VBO.h is a common header or will be included before AABB.h in cpp files.
#include "../VBO.h" // Or your actual path to Vertex struct definition

namespace Physics {

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    // Default constructor: creates an "invalid" or empty AABB
    AABB() : 
        min(std::numeric_limits<float>::max()), 
        max(std::numeric_limits<float>::lowest()) {}

    // Constructor from min and max points
    AABB(const glm::vec3& min_point, const glm::vec3& max_point) : min(min_point), max(max_point) {}

    /**
     * @brief Creates an AABB that encompasses all given vertices, optionally transformed.
     * @param vertices A vector of Vertex objects.
     * @param transform An optional transformation matrix to apply to vertices before calculating AABB.
     * @return The calculated AABB.
     */
    static AABB createFromVertices(const std::vector<Vertex>& vertices, const glm::mat4& transform = glm::mat4(1.0f)) {
        if (vertices.empty()) {
            return AABB(); // Return an invalid AABB if no vertices
        }

        glm::vec3 current_min(std::numeric_limits<float>::max());
        glm::vec3 current_max(std::numeric_limits<float>::lowest());

        for (const auto& vertex_obj : vertices) {
            glm::vec4 transformed_pos_h = transform * glm::vec4(vertex_obj.position, 1.0f);
            glm::vec3 transformed_pos = glm::vec3(transformed_pos_h) / transformed_pos_h.w; // Perspective divide

            current_min.x = std::min(current_min.x, transformed_pos.x);
            current_min.y = std::min(current_min.y, transformed_pos.y);
            current_min.z = std::min(current_min.z, transformed_pos.z);

            current_max.x = std::max(current_max.x, transformed_pos.x);
            current_max.y = std::max(current_max.y, transformed_pos.y);
            current_max.z = std::max(current_max.z, transformed_pos.z);
        }
        return AABB(current_min, current_max);
    }

    /**
     * @brief Transforms this AABB by a given matrix.
     * This method recomputes the AABB by transforming its 8 corner points
     * and finding the new min/max extents that enclose these transformed corners.
     * @param matrix The transformation matrix.
     * @return A new AABB that is the result of the transformation.
     */
    AABB transform(const glm::mat4& matrix) const {
        if (min.x > max.x) return AABB(); // Invalid AABB

        glm::vec3 corners[8] = {
            glm::vec3(min.x, min.y, min.z),
            glm::vec3(max.x, min.y, min.z),
            glm::vec3(min.x, max.y, min.z),
            glm::vec3(max.x, max.y, min.z),
            glm::vec3(min.x, min.y, max.z),
            glm::vec3(max.x, min.y, max.z),
            glm::vec3(min.x, max.y, max.z),
            glm::vec3(max.x, max.y, max.z)
        };

        glm::vec3 new_min(std::numeric_limits<float>::max());
        glm::vec3 new_max(std::numeric_limits<float>::lowest());

        for (int i = 0; i < 8; ++i) {
            glm::vec4 transformed_corner_h = matrix * glm::vec4(corners[i], 1.0f);
            glm::vec3 transformed_corner = glm::vec3(transformed_corner_h) / transformed_corner_h.w;

            new_min.x = std::min(new_min.x, transformed_corner.x);
            new_min.y = std::min(new_min.y, transformed_corner.y);
            new_min.z = std::min(new_min.z, transformed_corner.z);

            new_max.x = std::max(new_max.x, transformed_corner.x);
            new_max.y = std::max(new_max.y, transformed_corner.y);
            new_max.z = std::max(new_max.z, transformed_corner.z);
        }
        return AABB(new_min, new_max);
    }

    /**
     * @brief Checks if this AABB intersects with another AABB.
     * @param other The other AABB to check against.
     * @return True if they intersect, false otherwise.
     */
    bool intersects(const AABB& other) const {
        // Check for no overlap on any axis
        if (max.x < other.min.x || min.x > other.max.x) return false;
        if (max.y < other.min.y || min.y > other.max.y) return false;
        if (max.z < other.min.z || min.z > other.max.z) return false;
        return true; // Overlapping on all axes
    }

    /**
     * @brief Checks if a point is contained within this AABB.
     * @param point The point to check.
     * @return True if the point is inside or on the boundary, false otherwise.
     */
    bool contains(const glm::vec3& point) const {
        return (point.x >= min.x && point.x <= max.x &&
                point.y >= min.y && point.y <= max.y &&
                point.z >= min.z && point.z <= max.z);
    }

    /**
     * @brief Expands the AABB to include another AABB.
     * @param other The AABB to include.
     */
    void expand(const AABB& other) {
        if (other.min.x > other.max.x) return; // Other AABB is invalid

        if (min.x > max.x) { // This AABB was invalid, just copy other
            min = other.min;
            max = other.max;
            return;
        }

        min.x = std::min(min.x, other.min.x);
        min.y = std::min(min.y, other.min.y);
        min.z = std::min(min.z, other.min.z);

        max.x = std::max(max.x, other.max.x);
        max.y = std::max(max.y, other.max.y);
        max.z = std::max(max.z, other.max.z);
    }
};

} // namespace Physics

#endif // AABB_H
