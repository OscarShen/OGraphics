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
		position(position),front(glm::vec3(0.0f, 0.0f, -1.0f)), 
		movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {}

	glm::mat4 getViewMatrix() { return glm::lookAt(this->position, this->position + this->front, glm::vec3(0.0f, 1.0f, 0.0f));}

	void scroll(double xOffset, double yOffset) {
		std::cout << "scroll" << std::endl;
		zoom += yOffset;
		if (zoom < 1.0f)
			zoom = 1.0f;
		if (zoom > 45.0f)
			zoom = 45.0f;
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
	}
};

#endif // !Camera_h_
