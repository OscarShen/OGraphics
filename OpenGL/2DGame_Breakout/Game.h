#pragma once
#ifndef GAME_H__
#define GAME_H__

#include <GL45\glew.h>
#include <GLFW\glfw3.h>

// Game states
enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

// Game holds all game-related state and functionality.

class Game
{
public:
	// State
	GameState				state;
	// Record which keys are pressed
	GLboolean				keys[1024];
	// Parameters of window
	GLuint					width, height;
	Game(GLuint width, GLuint height);
	~Game();
	
	// Initialize game (Load all data to prepare a game)
	void init();
	// Game loop
	void processInput(GLfloat dt);
	void update(GLfloat dt);
	void render();
};
#endif // !GAME_H__
