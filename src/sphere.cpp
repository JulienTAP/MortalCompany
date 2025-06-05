#include "sphere.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <cmath>
#include <GL/glew.h>

Sphere::Sphere(Shader* shader_program, float radius, int sectors, int stacks, float scale_x, float scale_y)
    : Shape(shader_program), num_indices(0), VAO(0) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    //Génère les sommets
    for (int i = 0; i <= stacks; ++i) {
        float stack_angle = glm::pi<float>() / 2 - i * glm::pi<float>() / stacks;
        float xy = radius * cos(stack_angle);
        float z = radius * sin(stack_angle);

        for (int j = 0; j <= sectors; ++j) {
            float sector_angle = j * 2 * glm::pi<float>() / sectors;

            float x = xy * cos(sector_angle) * scale_x;
            float y = xy * sin(sector_angle) * scale_y;

            // Ajouter les sommets
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z); // Z reste inchangé pour conserver la hauteur
        }
    }

    //Génère les indices
    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1); 
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    num_indices = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, buffers);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Sphere::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);
    glBindVertexArray(VAO);

    Shape::draw(model, view, projection);

    glDrawElements(GL_TRIANGLE_STRIP, num_indices, GL_UNSIGNED_INT, nullptr);
}