#pragma once
#ifndef Camera_h_
#define Camera_h_
#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Defult camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera {
public:
	// Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	//glm::vec3 worldUp;
	// Eular angles
	GLfloat yaw; // left and right
	GLfloat pitch;// up and down
	// Camera options
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	// Constructor
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
		GLfloat yaw = YAW, GLfloat pitch = PITCH) :
		position(position),front(glm::vec3(0.0f, 0.0f, -1.0f)), up(up),yaw(yaw),pitch(pitch),
		movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {}

	glm::mat4 getViewMatrix() { 
		return glm::lookAt(position, position + front, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void scroll(double xOffset, double yOffset) {
		std::cout << "scroll" << std::endl;
		zoom += yOffset;
		if (zoom < 1.0f)
			zoom = 1.0f;
		if (zoom > 45.0f)
			zoom = 45.0f;
		updateCameraVectors();
	}

	void mouse(double xOffSet, double yOffSet)
	{
		std::cout << "mouse" << std::endl;
		xOffSet *= mouseSensitivity;
		yOffSet *= mouseSensitivity;

		yaw += xOffSet;
		pitch += yOffSet;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		updateCameraVectors();
	}

	void key(CameraMovement direction, GLfloat deltaTime) {
		GLfloat v = deltaTime * movementSpeed;
		if (direction == FORWARD)
			position += front*v;
		if (direction == BACKWARD)
			position -= front*v;
		if (direction == LEFT)
			position -= right*v;
		if (direction == RIGHT)
			position += right*v;
		updateCameraVectors();
		std::cout << front.x << " " << front.y << " " << front.z << std::endl;
	}

private:
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->right = glm::normalize(glm::cross(this->front, this->up));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
};

#endif // !Camera_h_
