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
#include "ai_list.h"
#include "bullet.h"
#include "collision.h"
#include "duck.h"
#include "hunter.h"
#include "level.h"
#include "log.h"
#include "render_list.h"
#include "world.h"

static SDL_Window*		window		= 0;
static SDL_GLContext	gl_context	= 0;

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

	// FOR TESTING.
	Level test_level;
	if (test_level.load("test_level.tmx") && test_level.init())
		Log::log(LOG_INFO, "Test level loaded and initialized successfully.");
	else
	{
		Log::log(LOG_ERROR, "Test level failed to load & init.");
		return 1;
	}

	// Init windowing and rendering.
	if (!Hunter::init_rendering())	return 1;
	if (!Bullet::init_rendering())	return 1;

	// Duck.
	Duck duck(100, 500);
	duck.init();

	// Hunters.
	Hunter hunter1(2000, 1000, 2000, 2200);
	Hunter hunter2(5000, 1000, 5000, 5400);
	Hunter hunter3(1600, 128, 1250, 3500);

	// Add renderables to the list.
	Render_List::get()->push_back(&duck);
	Render_List::get()->push_back(&hunter1);
	Render_List::get()->push_back(&hunter2);
	Render_List::get()->push_back(&hunter3);
	Render_List::get()->push_back(&test_level);

	// Add all AI controlled items to the list.
	AI_List::get()->push_back(&hunter1);
	AI_List::get()->push_back(&hunter2);
	AI_List::get()->push_back(&hunter3);

	//
	// Main loop of the game.
	//
	World::get()->SetContactListener(Contact_Listener::get());
	int running = 1;
	unsigned last_time = SDL_GetTicks();
	unsigned delta_time;
	unsigned now;
	while (running && Duck::get_duck()->is_alive())
	{
		SDL_Event event;
		if (SDL_PollEvent(&event))
			switch (event.type)
		{
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					duck.left();
					break;
				case SDLK_RIGHT:
					duck.right();
					break;
				case SDLK_SPACE:
					duck.jump();
					break;
				default:
					break;
				}
			default:
				break;
		}

		// Run AI.
		AI_List::get()->run_ai_for_all();

		// Render.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		duck.set_camera_to_duck();
		Render_List::get()->render_all();
		SDL_GL_SetSwapInterval(1);
		SDL_GL_SwapWindow(window);

		// Simulate physics.
		now = SDL_GetTicks();
		delta_time = now - last_time;
		last_time = now;
		World::get()->Step((float32)delta_time / 1000.0, 8, 3);
	}

	Log::log(LOG_INFO, "Exiting.");
	test_level.fini();
	duck.fini();
	Hunter::finish_rendering();
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
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);

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
