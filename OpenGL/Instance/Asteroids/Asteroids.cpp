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
#include "..\..\Model\Model.h"
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
	Model rock("resource/planet/rock.obj");
	Model planet("resource/planet/planet.obj");
	Shader planetShader("Instance/Asteroids/planet.vs", "Instance/Asteroids/planet.frag");
	Shader rockShader("Instance/Asteroids/rock.vs", "Instance/Asteroids/rock.frag");
	//Shader rockShader("Instance/Asteroids/planet.vs", "Instance/Asteroids/planet.frag");

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)screenWidth / screenHeight, 0.1f, 1000.0f);

	planetShader.use();
	glUniformMatrix4fv(glGetUniformLocation(planetShader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glm::mat4 PlanetModel;
	PlanetModel = glm::translate(PlanetModel, glm::vec3(0.0f, -5.0f, 0.0f));
	PlanetModel = glm::scale(PlanetModel, glm::vec3(4.0f, 4.0f, 4.0f));
	glUniformMatrix4fv(glGetUniformLocation(planetShader.program, "model"), 1, GL_FALSE, glm::value_ptr(PlanetModel));

	rockShader.use();
	glUniformMatrix4fv(glGetUniformLocation(rockShader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	// Generate a large list of semi-random PlanetModel transformation matrices
	GLuint amount = 10000;
	std::vector<glm::mat4> modelMatrices(amount);
	srand(glfwGetTime());
	GLfloat radius = 150.0f;
	GLfloat offset = 25.0f;
	for (GLuint i = 0; i < amount; ++i) {
		// 1. Translation: Randomly displace along circle with radius 'radius' in range [-offset, offset]
		GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = sin(angle)*radius + displacement;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y = -2.5f + displacement * 0.4f;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat z = cos(angle)*radius + displacement;
		glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(x, y, z));
		// 2. Scale: Scale between 0.05 and 0.25
		GLfloat scale = (rand() % 20) / 100.f + 0.05f;
		model = glm::scale(model, glm::vec3(scale));

		// 3. Rotation: add random rotation around a randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. Now add to list of matrices
		modelMatrices[i] = model;
	}

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	//GLuint blockIndex = glGetUniformBlockIndex(rockShader.program, "instanceMatrix");
	//GLint blockSize;
	//glGetActiveUniformBlockiv(rockShader.program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	//GLubyte *blockBuffer = (GLubyte*)malloc(blockSize);
	//memcpy(blockBuffer, modelMatrices.data(), blockSize);
	
	//glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_STATIC_DRAW);
	//glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, buffer);
	//free(blockBuffer);

	for (GLuint i = 0; i < rock.getMeshes().size(); ++i) {
		GLuint VAO = (rock.getMeshes())[i].getVAO();
		glBindVertexArray(VAO);
		// Set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glBindVertexArray(0);
	}

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
		planetShader.use();
		glUniformMatrix4fv(glGetUniformLocation(planetShader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		rockShader.use();
		glUniformMatrix4fv(glGetUniformLocation(rockShader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		planetShader.use();
		planet.draw(planetShader);

		rockShader.use();
		glBindTexture(GL_TEXTURE_2D, (rock.getTextureLoaded())[0].id);
		for (GLuint i = 0; i < rock.getMeshes().size(); ++i) {
			glBindVertexArray((rock.getMeshes())[i].getVAO());
			glDrawElementsInstanced(GL_TRIANGLES, (rock.getMeshes())[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}
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