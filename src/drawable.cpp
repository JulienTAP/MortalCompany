#include "drawable.h"

#include "glm/gtc/matrix_transform.hpp" // For glm::translate, glm::rotate, glm::scale
#include "glm/gtc/constants.hpp"      // For glm::radians


void Drawable::translate(glm::vec3 translation) {
    localModelMatrix = glm::translate(localModelMatrix, translation);
}

void Drawable::rotate(float degrees, glm::vec3 axis) {
    localModelMatrix = glm::rotate(localModelMatrix, glm::radians(degrees), axis);
}

void Drawable::scale(glm::vec3 scaleVec) {
    localModelMatrix = glm::scale(localModelMatrix, scaleVec);
}
