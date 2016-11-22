#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <memory>

#include <GL45\glew.h>
#include <GLFW\glfw3.h>

#include "..\..\Camera\Camera.h"
#include "..\..\Shader\Shader.h"
#include "..\..\Texture\Texture.h"
#include "..\..\ErrorCheck\ErrorCheck.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Properties
GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_Movement();

Camera camera;

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main() {
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// multisample
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight,
		"ILoveOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

#pragma region "object_initialization"
	GLfloat cubeVertices[] = {
		// Positions       
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Shader shader("Ex/MSAA/MSAA.vs", "Ex/MSAA/MSAA.frag");

	//GLuint VAO, VBO;
	//glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
	//glEnableVertexAttribArray(0);
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	GLint blockIndex = glGetUniformBlockIndex(shader.program, "vertexBlock");
	GLint blockSize;
	glGetActiveUniformBlockiv(shader.program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	//GLfloat *buffer = (GLfloat*)malloc(blockSize);
	//for (int i = 0; i < 36; ++i) {
	//	memcpy(buffer + i * 4, cubeVertices + i * 3, 3*sizeof(GL_FLOAT));
	//}
	GLuint ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);

	glBufferData(GL_UNIFORM_BUFFER, blockSize, NULL, GL_DYNAMIC_DRAW);
	GLfloat *uboMap = (GLfloat*) glMapBufferRange(GL_UNIFORM_BUFFER, 0, blockSize, GL_MAP_WRITE_BIT);
	for (int i = 0; i < 36; ++i) {
		memcpy(uboMap + i * 4, cubeVertices + i * 3, 3 * sizeof(GL_FLOAT));
	}
	uboMap = nullptr;
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	//glBufferData(GL_UNIFORM_BUFFER, blockSize, buffer, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, ubo);

	//free(buffer);

	shader.use();
	glm::mat4 model;
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)(screenWidth) / screenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// bind a vao to prevent error of drawArray
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnable(GL_DEPTH_TEST);

#pragma endregion
	int count = 0;
	while (!glfwWindowShouldClose(window)) {
		// Set frame time
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		if (++count % 120 == 0)
			std::cout << static_cast<int>(1 / deltaTime) << std::endl;

		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region "render"
		glm::mat4 view = camera.getViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);
#pragma endregion
		// Check and call events
		glfwPollEvents();
		do_Movement();

		// Swap the buffers
		glfwSwapBuffers(window);
	}
#pragma region "free"

#pragma endregion
	glfwTerminate();
	return 0;
}

#pragma region "User input"
// Moves/alters the camera positions based on user input
void do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.do_key(CameraMovement::MOVEFORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.do_key(CameraMovement::MOVEBACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.do_key(CameraMovement::MOVELEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.do_key(CameraMovement::MOVERIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)(xpos - lastX);
	GLfloat yoffset = (GLfloat)(lastY - ypos);

	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	camera.do_mouse(xoffset, yoffset);
}
#pragma endregion