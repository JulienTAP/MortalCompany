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
    void draw(Camera& camera, const glm::mat4& parentTransform);
    void key_handler(int key) const;
    void transform(const glm::mat4 &transform) { transform_ = transform_ * transform; }
    void setTransform(const glm::mat4& transform) { transform_ = transform; }

private:
    glm::mat4 transform_;
    std::vector<Node *> children_;
    std::vector<Mesh *> children_mesh_;
};
