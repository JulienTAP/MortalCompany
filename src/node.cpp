#include "node.h"
#include "mesh.h"
#include <iostream>

Node::Node(const glm::mat4 &transform) : transform_(transform)
{
    children_ = std::vector<Node *>();
}

void Node::add(Node *node)
{
    children_.push_back(node);
}

void Node::add(Mesh *mesh)
{
    children_mesh_.push_back(mesh);
}

void Node::draw(Camera& camera, const glm::mat4& parentTransform)
{
    glm::mat4 modelMatrix = parentTransform * transform_;

    for (auto* mesh : children_mesh_)
    {
        mesh->shader.Activate();
        glUniformMatrix4fv(glGetUniformLocation(mesh->shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

        mesh->Draw(camera);
    }

    for (auto* child : children_)
    {
        child->draw(camera, modelMatrix);
    }
}

void Node::key_handler(int key) const
{
    for (const auto &child : children_)
    {
        child->key_handler(key);
    }
}
