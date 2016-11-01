#include <string>

// GLEW
#include <GL45\glew.h>

// GLFW
#include <GLFW\glfw3.h>

// GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// SOIL
#include <SOIL.h>

// GL includes
#include "..\Camera\Camera.h"
#include "..\Shader\Shader.h"
#include "Model.h"

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

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Setup and compile our shaders
	Shader shader("Model/vertex.vs", "Model/fragment.frag");
	Model myModel("Model/resource/nanosuit.obj");

	// Point light positions
	glm::vec3 pointLightPositions[] = {
		glm::vec3(2.3f,-1.6f,-3.0f),
		glm::vec3(-1.7f,0.9f,1.0f)
	};

	while (!glfwWindowShouldClose(window)) {
		// Set frame time
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		std::cout << (int)(1/deltaTime) << std::endl;

		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		// Transformation matrices
		glm::mat4 projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Set the lighting uniforms
		glUniform3f(glGetUniformLocation(shader.program, "viewPos"), camera.position.x, camera.position.y, camera.position.z);
		// Point light 1
		glUniform3f(glGetUniformLocation(shader.program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(shader.program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader.program, "pointLights[0].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(shader.program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader.program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(shader.program, "pointLights[0].linear"), 0.009f);
		glUniform1f(glGetUniformLocation(shader.program, "pointLights[0].quadratic"), 0.0032f);
		// Point light 2
		glUniform3f(glGetUniformLocation(shader.program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(shader.program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader.program, "pointLights[1].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(shader.program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader.program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(shader.program, "pointLights[1].linear"), 0.009f);
		glUniform1f(glGetUniformLocation(shader.program, "pointLights[1].quadratic"), 0.0032f);

		// Draw the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		myModel.draw(shader);

		// Check and call events
		glfwPollEvents();
		do_Movement();

		// Swap the buffers
		glfwSwapBuffers(window);
	}
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
