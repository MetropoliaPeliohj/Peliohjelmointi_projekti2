#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <windows.h>
#include <gl/GL.h>


/**
	Data type for representing a point in 3D space.
*/
typedef struct
{
	// Location
	GLfloat	_x;
	GLfloat	_y;
	GLfloat	_z;

	// Texture coordinates
	GLfloat	_s;
	GLfloat	_t;
}
Vertex;

#endif // _VERTEX_H_
