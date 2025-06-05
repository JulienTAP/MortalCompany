#include "drawable.h"

#include "glm/gtc/matrix_transform.hpp" // For glm::translate, glm::rotate, glm::scale
#include "glm/gtc/constants.hpp"      // For glm::radians


void Drawable::translate(const glm::vec3& translation) { // Added const&
    localModelMatrix = glm::translate(localModelMatrix, translation);
}

void Drawable::rotate(float degrees, const glm::vec3& axis) { // Added const&
    localModelMatrix = glm::rotate(localModelMatrix, glm::radians(degrees), axis);
}

void Drawable::scale(const glm::vec3& scaleVec) { // Added const&
    localModelMatrix = glm::scale(localModelMatrix, scaleVec);
}
