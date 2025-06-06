#include "camera.h"



Camera::Camera(int width, int height, glm::vec3 position, float FOVdeg, float nearPlane, float farPlane)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane, const glm::vec3& target)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	view = glm::lookAt(Position, target, up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}


void Camera::Inputs(GLFWwindow *window) {
	return;
}


//
// void Camera::Inputs(GLFWwindow* window)
// {
// 	// Handles key inputs
// 	glm::vec3 direction(0.0f);
// 	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
// 	{
// 		direction += Orientation;
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
// 	{
// 		direction += -glm::normalize(glm::cross(Orientation, Up));
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
// 	{
// 		direction += -Orientation;
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
// 	{
// 		direction += glm::normalize(glm::cross(Orientation, Up));
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
// 	{
// 		direction += Up;
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
// 	{
// 		direction += -Up;
// 	}
//
// 	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
// 	{
// 		speed = 0.06f;
// 	}
// 	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
// 	{
// 		speed = 0.03f;
// 	}
//
// 	// Normalize the direction vector to have consistent speed
// 	if (glm::length(direction) > 0.0f)
// 	{
// 		direction = glm::normalize(direction);
// 	}
//
// 	glm::vec3 velocity = direction * speed;
// 	glm::vec3 newPos = Position + velocity;
//
// 	if (isPositionValid(newPos))
// 	{
// 		Position = newPos;
// 	}
// 	else
// 	{
// 		// Slide along walls by trying each axis separately
// 		glm::vec3 tempPos = Position;
// 		tempPos.x += velocity.x;
// 		if (isPositionValid(tempPos)) {
// 			Position.x = tempPos.x;
// 		}
//
// 		tempPos = Position;
// 		tempPos.z += velocity.z;
// 		if (isPositionValid(tempPos)) {
// 			Position.z = tempPos.z;
// 		}
//
// 		tempPos = Position;
// 		tempPos.y += velocity.y;
// 		if(isPositionValid(tempPos)) {
// 			Position.y = tempPos.y;
// 		}
// 	}
//
//
// 	// Handles mouse inputs
// 	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
// 	{
// 		// Hides mouse cursor
// 		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
//
// 		// Prevents camera from jumping on the first click
// 		if (firstClick)
// 		{
// 			glfwSetCursorPos(window, (width / 2), (height / 2));
// 			firstClick = false;
// 		}
//
// 		// Stores the coordinates of the cursor
// 		double mouseX;
// 		double mouseY;
// 		// Fetches the coordinates of the cursor
// 		glfwGetCursorPos(window, &mouseX, &mouseY);
//
// 		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
// 		// and then "transforms" them into degrees
// 		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
// 		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;
//
// 		// Calculates upcoming vertical change in the Orientation
// 		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
//
// 		// Decides whether or not the next vertical Orientation is legal or not
// 		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
// 		{
// 			Orientation = newOrientation;
// 		}
//
// 		// Rotates the Orientation left and right
// 		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
//
// 		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
// 		glfwSetCursorPos(window, (width / 2), (height / 2));
// 	}
// 	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
// 	{
// 		// Unhides cursor since camera is not looking around anymore
// 		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
// 		// Makes sure the next time the camera looks around it doesn't jump
// 		firstClick = true;
// 	}
// }

bool Camera::isPositionValid(const glm::vec3& pos) {
	const float horizontalPadding = 0.2f;
	const float verticalPadding = 0.4f;

	const float minY = -1.0f + verticalPadding;
	const float maxY =  1.0f - verticalPadding;

	bool inMainRoom = (pos.x >= -3.5f + horizontalPadding && pos.x <= 3.5f - horizontalPadding) &&
					  (pos.y >= minY && pos.y <= maxY) &&
					  (pos.z >= -3.5f + horizontalPadding && pos.z <= 3.5f);

	bool inCorridor = (pos.x >= -0.5f + horizontalPadding && pos.x <= 0.5f - horizontalPadding) &&
					  (pos.y >= minY && pos.y <= maxY) &&
					  (pos.z >= 3.5f && pos.z <= 6.0f);

	bool inRoom2 =    (pos.x >= -3.0f + horizontalPadding && pos.x <= 3.0f - horizontalPadding) &&
					  (pos.y >= minY && pos.y <= maxY) &&
					  (pos.z >= 6.0f && pos.z <= 10.0f - horizontalPadding);

	return inMainRoom || inCorridor || inRoom2;
}