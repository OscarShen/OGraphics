#include <iostream>
#include <algorithm>

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
#include "../../Shader/Shader.h"

// Camera
#include "../../Camera/Camera.h"

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
// Light attributes

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ILoveOpenGL!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glViewport(0, 0, WIDTH, HEIGHT);

	glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
	glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
	glOrtho(0, WIDTH, 0, HEIGHT, 0, 1); // essentially set coordinate system
	glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
	glLoadIdentity(); // same as above comment


	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	glewInit();


	glEnable(GL_DEPTH_TEST);

	Shader containerShader("Lighting/container.vs", "Lighting/container.frag");
	Shader lightShader("Lighting/lamp.vs", "Lighting/lamp.frag");

	//////////////////////// Init //////////////////////////////////

	GLfloat vertices[] = {
		-1.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		1.0f,1.0f,0.0f
	};

	GLuint indices[] = {
		0,1,2,
		2,3,0
	};

	GLuint atomicCounterArrayBlue[1] = { 0 };
	GLuint atomicCounterArrayRedAndGreen[2] = { 0,0 };

	GLuint VBO, EBO, ATOMIC_BUFFER_RED_GREEN, ATOMIC_BUFFER_BLUE;
	GLint invNumPixelLocation;
	Shader shader("", "");

	glCreateBuffers(1, &VBO);
	glNamedBufferStorage(VBO, sizeof(vertices), vertices, 0);

	glCreateBuffers(1, &EBO);
	glNamedBufferStorage(EBO, sizeof(indices), indices, 0);

	glCreateBuffers(1, &ATOMIC_BUFFER_RED_GREEN);
	glNamedBufferStorage(ATOMIC_BUFFER_RED_GREEN, sizeof(GLuint) * 2, atomicCounterArrayRedAndGreen, 0);
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint) * 2, atomicCounterArrayRedAndGreen);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, ATOMIC_BUFFER_RED_GREEN);

	glCreateBuffers(1, &ATOMIC_BUFFER_BLUE);
	glNamedBufferStorage(ATOMIC_BUFFER_BLUE, sizeof(GLuint), atomicCounterArrayBlue, 0);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, ATOMIC_BUFFER_BLUE);

	invNumPixelLocation = glGetUniformLocation(shader.program, "invNumPixel");
	glProgramUniform1f(shader.program, invNumPixelLocation, 1.0f / (800.0f * 600.0f));

	GLfloat zoom = -10.0f;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		GLfloat currenttime = (GLfloat)glfwGetTime();
		deltatime = currenttime - lasttime;
		lasttime = currenttime;
		do_movement();

		glLoadIdentity();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLuint *userCounters;
		userCounters = (GLuint *)glMapNamedBufferRange(ATOMIC_BUFFER_BLUE,
			0, sizeof(GLuint), GL_MAP_WRITE_BIT);
		atomicCounterArrayBlue[0] = 0;

		glNamedBufferSubData(ATOMIC_BUFFER_BLUE, 0, sizeof(GLuint), atomicCounterArrayBlue);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_INDEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glIndexPointer(GL_UNSIGNED_INT, 0, 0);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);

		glfwSwapBuffers(window);

		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, ATOMIC_BUFFER_BLUE);
		GLubyte *src = (GLubyte*) glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_READ_ONLY);
		if (src)
			memcpy((void*)atomicCounterArrayBlue, src, 4);
		glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

		unsigned int numPixel = std::max(1u, atomicCounterArrayBlue[0]);

		GLint invNumPixelLocation;
		invNumPixelLocation = glGetUniformLocation(shader.program, "invNumPixel");
		GLfloat invNumPixelValue;
		glGetUniformfv(shader.program, invNumPixelLocation, &invNumPixelValue);
		glProgramUniform1f(shader.program, invNumPixelLocation, 1.0f / static_cast<float>(numPixel));

		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, ATOMIC_BUFFER_RED_GREEN);
		userCounters = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER,
			0, sizeof(GLuint) * 2, GL_MAP_READ_BIT);
	}
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