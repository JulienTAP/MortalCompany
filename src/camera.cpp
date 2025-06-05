#include "camera.h"
#include "physics/AABB.h" // Include for AABB struct and methods
#include "drawable.h"     // Include for Drawable class (to access getWorldAABB, isCollidable)
#include <iostream>       // For debug output

Camera::Camera(int width, int height, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
    // FOVdeg, nearPlane, farPlane are passed to updateMatrix, no need to store if not used elsewhere
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window, const std::vector<Drawable*>& collidableObjects, float deltaTime)
{
    float currentSpeed = speed; // Base speed
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		currentSpeed = 0.06f; // Sprint speed
	}
    currentSpeed *= deltaTime * 60.0f; // Scale speed by deltaTime, assuming base speed is for 60FPS

    glm::vec3 displacement = glm::vec3(0.0f);

	// Handle key inputs for desired displacement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		displacement += currentSpeed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		displacement += currentSpeed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		displacement += currentSpeed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		displacement += currentSpeed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		displacement += currentSpeed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		displacement += currentSpeed * -Up;
	}

    if (glm::length(displacement) > 0.0f) // Only check collision if there's movement
    {
        glm::vec3 potentialPosition = Position + displacement;

        // Define the camera's AABB at the potential position
        // The camera's AABB is centered on its Position.
        Physics::AABB cameraAABB(potentialPosition - collisionSize,
                                 potentialPosition + collisionSize);

        bool collisionDetected = false;
        for (const auto& obj : collidableObjects) {
            if (obj && obj->isCollidable()) {
                // Assuming collidable objects are direct children of scene root,
                // so parentWorldMatrix for them is identity.
                Physics::AABB objectWorldAABB = obj->getWorldAABB(glm::mat4(1.0f));
                if (cameraAABB.intersects(objectWorldAABB)) {
                    collisionDetected = true;
                    // std::cout << "Collision with an object!" << std::endl; // Debug output
                    break;
                }
            }
        }

        if (!collisionDetected) {
            Position = potentialPosition;
        } else {
            // More sophisticated collision response could be implemented here,
            // e.g., trying to move along axes separately or sliding.
            // For now, we just prevent movement if any collision occurs.

            // Attempt to move along X and Z axes first (horizontal plane)
            glm::vec3 horizontalDisplacement = glm::vec3(displacement.x, 0.0f, displacement.z);
            if (glm::length(horizontalDisplacement) > 0.0f) {
                potentialPosition = Position + horizontalDisplacement;
                Physics::AABB camHorizontalAABB(potentialPosition - collisionSize, potentialPosition + collisionSize);
                bool horizontalCollision = false;
                for (const auto& obj : collidableObjects) {
                    if (obj && obj->isCollidable()) {
                        Physics::AABB objectWorldAABB = obj->getWorldAABB(glm::mat4(1.0f));
                        if (camHorizontalAABB.intersects(objectWorldAABB)) {
                            horizontalCollision = true;
                            break;
                        }
                    }
                }
                if (!horizontalCollision) {
                    Position = potentialPosition;
                }
            }

            // Then attempt to move along Y axis (vertical)
            glm::vec3 verticalDisplacement = glm::vec3(0.0f, displacement.y, 0.0f);
             if (glm::length(verticalDisplacement) > 0.0f) {
                potentialPosition = Position + verticalDisplacement; // Check from current (possibly updated horizontally) position
                Physics::AABB camVerticalAABB(potentialPosition - collisionSize, potentialPosition + collisionSize);
                bool verticalCollision = false;
                for (const auto& obj : collidableObjects) {
                     if (obj && obj->isCollidable()) {
                        Physics::AABB objectWorldAABB = obj->getWorldAABB(glm::mat4(1.0f));
                        if (camVerticalAABB.intersects(objectWorldAABB)) {
                            verticalCollision = true;
                            break;
                        }
                    }
                }
                if (!verticalCollision) {
                    Position = potentialPosition;
                }
            }
        }
    }


	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		if (firstClick)
		{
			glfwSetCursorPos(window, (double)width / 2.0, (double)height / 2.0);
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2.0)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2.0)) / width;
        // Apply deltaTime to rotation as well for frame-rate independence
        rotX *= deltaTime * 2.0f; // Sensitivity might need re-tuning
        rotY *= deltaTime * 2.0f;


		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
		glfwSetCursorPos(window, (double)width / 2.0, (double)height / 2.0);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}
