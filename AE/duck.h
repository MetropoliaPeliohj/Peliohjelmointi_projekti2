#ifndef _DUCK_H_
#define _DUCK_H_

#include <windows.h>
#include <gl/GL.h>
#include "physable.h"
#include "renderable.h"


/**
	Player controlled duck.
*/
class Duck : public Renderable, public Physable
{
public:

	Duck(int, int);
	virtual ~Duck();

	// Access to (singleton) instance.
	static Duck* get_duck();

	// Renderable implementation.
	virtual void render();

	// Rendering initialization.
	static int	init();
	static void	fini();

	// Duck control and health.
	void left();
	void right();
	void jump();
	int is_alive() const	{ return m_is_alive; }
	void kill()				{ m_is_alive = 0; }

	// Camera will follow the duck.
	void set_camera_to_duck() const;

private:

	// Coordinates (this will change once
	// we get physics in).
	int _x;
	int _y;

	// Display List for rendering.
	static GLuint m_dl;

	// For (singleton) instance access.
	static Duck* m_instance;

	// Duck health, set to 0 if hit.
	int m_is_alive;
};

#endif // _DUCK_H_
