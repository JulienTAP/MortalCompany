#pragma once

#include "shape.h"
#include <GL/glew.h>

class Sphere : public Shape {
public:
    Sphere(Shader* shader_program, float radius = 1.0f, int sectors = 36, int stacks = 18, float scale_x = 1.0f, float scale_y = 1.0f);
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    GLuint VAO;              
    GLuint buffers[2];       
    unsigned int num_indices;
};