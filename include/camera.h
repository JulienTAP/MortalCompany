
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera
{
public:
    glm::vec3 Position;
    //glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    //glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    bool firstClick = true;

    int width;
    int height;

    // Adjust the speed of the camera and it's sensitivity when looking around
    float speed = 0.05f;
    float sensitivity = 50.0f;

    Camera(int width, int height, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane);

    void updateMatrix(float FOVdeg, float nearPlane, float farPlane, const glm::vec3& target);
    // Exports the camera matrix to a shader
    void Matrix(Shader &shader, const char *uniform);
    // Handles camera inputs
    void Inputs(GLFWwindow *window);

    bool isPositionValid(const glm::vec3& pos);
};
#endif
