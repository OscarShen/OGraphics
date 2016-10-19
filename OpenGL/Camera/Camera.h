#pragma once
#ifndef MyCamera_h_
#define MyCamera_h_

#include <GL\GL.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\glm.hpp>

enum CameraMovement
{
	MOVEFORWARD,
	MOVEBACKWARD,
	MOVELEFT,
	MOVERIGHT
};

glm::vec3 POSITION(0.0f, 0.0f, 3.0f);
glm::vec3 UP(0.0f, 1.0f, 0.0f);
glm::vec3 FRONT(0.0f, 0.0f, -1.0f);
glm::vec3&& RIGHT = glm::normalize(glm::cross(UP, POSITION + FRONT));

GLfloat PITCH(0.0f);
GLfloat YAW(-90.0f);

GLfloat MOVESPEED = 3.0f;
GLfloat MOUSESENSITIVITY = 0.25f;

class Camera {
public:
	// vectors of camera
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 front;

	GLfloat pitch;
	GLfloat yaw;

	GLfloat mouseSensitivity;

	// angles

	GLfloat moveSpeed;

	Camera() :position(POSITION), up(UP), right(RIGHT), front(FRONT), moveSpeed(MOVESPEED),
		pitch(PITCH), yaw(YAW), mouseSensitivity(MOUSESENSITIVITY) {}

	void do_key(CameraMovement direction, GLfloat deltatime) {
		GLfloat v = deltatime*moveSpeed;
		switch (direction)
		{
		case MOVEFORWARD:
			position += front*v;
			break;
		case MOVEBACKWARD:
			position -= front*v;
			break;
		case MOVELEFT:
			position -= right*v;
			break;
		case MOVERIGHT:
			position += right*v;
			break;
		default:
			break;
		}
	}

	void do_mouse(double xOffSet, double yOffSet) {
		xOffSet *= mouseSensitivity;
		yOffSet *= mouseSensitivity;

		pitch += (GLfloat)yOffSet;
		yaw += (GLfloat)xOffSet;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		updateVectors();
	}

	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}

private:
	void updateVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(pitch))*cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch))*sin(glm::radians(yaw));
		this->front = glm::normalize(front);
		this->right = glm::normalize(glm::cross(this->front, this->up));
	}
};
#endif // !MyCamera_h_
