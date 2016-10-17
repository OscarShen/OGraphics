#include <iostream>

// GLEW
//#define GLEW_STATIC
#include <GL45\glew.h>

// GLFW
#include <GLFW\glfw3.h>

// GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Shader
#include "../Shader/Shader.h"

// Camera
#include "MyCamera.h"

// Function prototys //////////////////////
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow * window, double xPos, double yPos);
void do_movement();

// GLOBAL /////////////////
const GLuint WIDTH = 800, HEIGHT = 600;
bool keys[2014];// Record the keys user pressed.
double lastX = 0, lastY = 0;
Camera camera;
GLfloat deltatime;
GLfloat lasttime = (GLfloat)glfwGetTime();

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ILoveOpenGL!", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);

	Shader containerShader("Lighting/container.vs", "Lighting/container.frag");
	Shader lightShader("Lighting/lamp.vs", "Lighting/lamp.frag");

	// The cube
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	GLuint containerVAO, VBO;
	glCreateBuffers(1, &VBO);
	glCreateVertexArrays(1, &containerVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint lightVAO;
	glCreateVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		GLfloat currenttime = (GLfloat)glfwGetTime();
		deltatime = currenttime - lasttime;
		lasttime = currenttime;

		do_movement();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Container Position
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(0.5f));

		glm::mat4 view;
		view = camera.getViewMatrix();

		glm::mat4 projection;
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / HEIGHT, 1.0f, 1000.0f);

		containerShader.use();
		auto comtainerModel = glGetUniformLocation(containerShader.program, "model");
		glUniformMatrix4fv(comtainerModel, 1, GL_FALSE, glm::value_ptr(model));
		auto containerView = glGetUniformLocation(containerShader.program, "view");
		glUniformMatrix4fv(containerView, 1, GL_FALSE, glm::value_ptr(view));
		auto containerProj = glGetUniformLocation(containerShader.program, "projection");
		glUniformMatrix4fv(containerProj, 1, GL_FALSE, glm::value_ptr(projection));

		// Container and light colors
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
		glUniform3fv(glGetUniformLocation(containerShader.program, "lightColor"), 1, glm::value_ptr(lightColor));

		glm::vec3 containerColor(1.0f, 0.5f, 0.31f);
		glUniform3fv(glGetUniformLocation(containerShader.program, "containerColor"), 1, glm::value_ptr(containerColor));

		
		glBindVertexArray(containerVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Light position
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));

		lightShader.use();
		auto lightModel = glGetUniformLocation(lightShader.program, "model");
		glUniformMatrix4fv(lightModel, 1, GL_FALSE, glm::value_ptr(model));
		auto lightView = glGetUniformLocation(lightShader.program, "view");
		glUniformMatrix4fv(lightView, 1, GL_FALSE, glm::value_ptr(view));
		auto lightProj = glGetUniformLocation(lightShader.program, "projection");
		glUniformMatrix4fv(lightProj, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &containerVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

bool firstMouse = true;
void mouse_callback(GLFWwindow * window, double xPos, double yPos)
{
	std::cout << "mouse" << std::endl;
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	double xOffSet, yOffSet;
	xOffSet = xPos - lastX;
	yOffSet = lastY - yPos;

	lastX = xPos;
	lastY = yPos;
	
	camera.do_mouse(xOffSet, yOffSet);
}

void do_movement() {
	if (keys[GLFW_KEY_W])
		camera.do_key(CameraMovement::MOVEFORWARD, deltatime);
	if (keys[GLFW_KEY_S])
		camera.do_key(CameraMovement::MOVEBACKWARD, deltatime);
	if (keys[GLFW_KEY_D])
		camera.do_key(CameraMovement::MOVERIGHT, deltatime);
	if (keys[GLFW_KEY_A])
		camera.do_key(CameraMovement::MOVELEFT, deltatime);
}