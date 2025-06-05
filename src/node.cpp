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

void Node::draw(Camera& camera)
{
    for(auto child : children_)
    {
        // Apply the transformation to the child node
        child->transform(transform_);
        // Draw the child node
        child->draw(camera);
    }
    for (auto child : children_mesh_)
    {
        child->Draw(camera);
    }
}

void Node::key_handler(int key) const
{
    for (const auto &child : children_)
    {
        child->key_handler(key);
    }
}
