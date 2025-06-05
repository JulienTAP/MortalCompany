// Example: light.h
#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"

struct LightProperties {
    glm::vec3 position;
    glm::vec4 color;

    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;
};

#endif // LIGHT_H