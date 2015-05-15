#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <string>
#include <vector>
#include <windows.h>
#include <gl/GL.h>
#include <SDL.h>
#include "vertex.h"
#include "renderable.h"


/**
	Tile-based level.
*/
class Level : public Renderable
{
public:

	Level();
	virtual ~Level();

	int	init();
	void fini();
	int load(const char*);

	// Renderable implementation.
	virtual void render();

private:

	// Level data.
	std::vector<Vertex>		m_tile_coords;
	std::string				m_image_file_name;

	// For rendering.
	GLuint					m_vbo;
	GLuint					m_tex;

};

#endif // _LEVEL_H_
