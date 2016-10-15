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

GLfloat MOVESPEED = 3.0f;

class Camera {
public:
	// vectors of camera
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 front;

	// angles

	GLfloat moveSpeed;

	Camera() :position(POSITION), up(UP), right(RIGHT), front(FRONT), moveSpeed(MOVESPEED) {}

	void do_key(CameraMovement direction, GLfloat deltatime) {
		GLfloat v = deltatime*moveSpeed;
		if (direction == CameraMovement::MOVEFORWARD)
			position += front*v;
		if (direction == CameraMovement::MOVEBACKWARD)
			position -= front*v;
		if (direction == CameraMovement::MOVERIGHT)
			position += right*v;
		if (direction == CameraMovement::MOVELEFT)
			position -= right*v;
	}

	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}

private:
	void update() {

	}
};


#endif // !MyCamera_h_
