#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <vector> // Required for std::vector

#include "shaderClass.h"
// Forward declare Drawable to avoid full include if not necessary,
// but for vector<Drawable*>, we might need the full definition or smart pointers.
// For simplicity with raw pointers here, forward declaration is fine.
class Drawable;
// We'll need AABB for collision checking logic, typically in .cpp
// but if Camera itself has an AABB or uses AABB methods directly in header, include it.
// For now, we'll include it in camera.cpp where the logic resides.


class Camera
{
public:
    // Stores the main vectors of the camera
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    // Prevents the camera from jumping around when first clicking left click
    bool firstClick = true;

    // Stores the width and height of the window
    int width;
    int height;

    // Adjust the speed of the camera and it's sensitivity when looking around
    float speed = 0.05; // Adjusted default speed
    float sensitivity = 1500.0f; // Increased sensitivity to compensate for deltaTime scaling

    // Dimensions of the camera's collision bounding box (half-extents)
    glm::vec3 collisionSize = glm::vec3(0.2f, 0.5f, 0.2f); // Example size (width, height, depth)/2

    // Camera constructor to set up initial values
    Camera(int width, int height, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane);

    // Updates the cameraMatrix
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    // Exports the camera matrix to a shader
    void Matrix(Shader &shader, const char *uniform);
    // Handles camera inputs and collision detection
    void Inputs(GLFWwindow *window, const std::vector<Drawable*>& collidableObjects, float deltaTime);
};
#endif
