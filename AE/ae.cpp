#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <Box2D/Box2D.h>

#include "ae.h"
#include "log.h"

static SDL_Window*		window = 0;
static SDL_GLContext	gl_context = 0;

int init_game();
int fini_game();


/**
A N K A N   E L A M A A

Peliohjelmointi 2015K
Metropolia AMK

Main program
*/
int main(int argc, char* argv[])
{
	if (!init_game())
	{
		Log::log(LOG_ERROR, "Failed to initalize game.");
		return 1;
	}
	Log::log(LOG_INFO, "Game initialized.");

	//
	// Main loop of the game.
	//
	SDL_Event event;
	while (1)
	{
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//
		// TODO: RENDERING CODE HERE.
		//

		// Double buffering in vsync.
		SDL_GL_SetSwapInterval(1);
		SDL_GL_SwapWindow(window);
	}

	Log::log(LOG_INFO, "Exiting.");
	if (!fini_game())
		return 1;

	return 0;
}


/**
Global initializations.

\return		1 for success, 0 otherwise
*/
int init_game()
{
	// RNG seed.
	srand((unsigned)time(0));

	// For SDL initialization and window creation, see
	// https://wiki.libsdl.org/SDL_CreateWindow
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		Log::log(LOG_ERROR, "SDL initialization failed.");
		return 0;
	}
	Log::log(LOG_INFO, "SDL initialized successfully.");
	window = SDL_CreateWindow(
		WINDOW_TITLE,					// window title
		SDL_WINDOWPOS_UNDEFINED,		// initial x position
		SDL_WINDOWPOS_UNDEFINED,		// initial y position
		WIDTH,							// width, in pixels
		HEIGHT,							// height, in pixels
		SDL_WINDOW_OPENGL				// flags
		);
	if (!window)
	{
		Log::log(LOG_ERROR, "Could not create window.");
		return 0;
	}
	Log::log(LOG_INFO, "Window created.");


	// For OpenGL context creation, see
	// https://wiki.libsdl.org/SDL_GL_CreateContext
	gl_context = SDL_GL_CreateContext(window);
	if (!gl_context)
	{
		Log::log(LOG_ERROR, "OpenGL context creation failed.");
		return 0;
	}
	Log::log(LOG_INFO, "OpenGL context created.");

	// For GLEW initialization, see
	// http://glew.sourceforge.net/basic.html
	if (glewInit() != GLEW_OK)
	{
		Log::log(LOG_ERROR, "GLEW initialization failed.");
		return 0;
	}
	Log::log(LOG_INFO, "GLEW initialized successfully.");

	// OpenGL stuff
	glClearColor(CLEAR_COLOR);
	glColor3f(DRAW_COLOR);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, WIDTH, 0.0f, HEIGHT, 0.0f, 10.0f);
	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(0.15, 0.15, 0.15);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return 1;
}


/**
Global finalizations.

\return		1 for success, 0 otherwise
*/
int fini_game()
{
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 1;
}
