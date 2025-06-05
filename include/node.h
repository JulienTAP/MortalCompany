#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "mesh.h"
#include "shaderClass.h"
#include "camera.h"

class Shape;

class Node
{
public:
    Node(const glm::mat4 &transform = glm::mat4(1.0f));
    void add(Node *node);
    void add(Mesh *mesh);
    void add(Shape *shape);
    void draw(Camera& camera);
    void key_handler(int key) const;
    void transform(const glm::mat4 &transform) { transform_ = transform_ * transform; }

private:
    glm::mat4 transform_;
    std::vector<Node *> children_;
    std::vector<Mesh *> children_mesh_;
    std::vector<Shape *> children_shape_;
};
