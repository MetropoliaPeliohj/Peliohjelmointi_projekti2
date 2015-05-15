#ifndef _AE_H_
#define _AE_H_

#define DEG2RAD(D)			((D) * 0.0174532925)
#define RAD2DEG(R)			((R) * 57.2957795)
#define BUFFER_OFFSET(i)	((void*)(i))


#define WIDTH				1024
#define HEIGHT				768
#define WINDOW_TITLE		"Ankan Elamaa"
#define CLEAR_COLOR			0, 0, 0, 0
#define DRAW_COLOR			1.0, 1.0, 1.0
#define LOG_INFO			0
#define LOG_WARNING			1
#define LOG_ERROR			2
#define DUCK_RADIUS			20
#define PHYS_SCALE			100
#define TIMESTEP			(1.0 / 60.0)
#define GRAVITY				-10.0f
#define DUCK_ORIG_X			1000
#define DUCK_ORIG_Y			1000
#define DUCK_RADIUS			20
#define DUCK_FORCE_LEFT		-5
#define DUCK_FORCE_RIGHT	5
#define DUCK_FORCE_JUMP		40
#define DUCK_DENSITY		0.9f
#define DUCK_FRICTION		0.8f
#define HUNTER_WIDTH		128
#define	HUNTER_HEIGHT		128
#define	HUNTER_DENSITY		1.0f
#define HUNTER_FRICTION		1.0f
#define HUNTER_MOVING_P		0.9f
#define HUNTER_SHOOTING_P	0.1f
#define HUNTER_SHOOT_DSTNC	1000
#define HUNTER_SPEED		1
#define BULLET_RADIUS		5
#define BULLET_DENSITY		1.0f
#define BULLET_FRICTION		0.8f
#define BULLET_FORCE		5

#endif
