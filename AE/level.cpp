#include <string>
#include <vector>
#include <windows.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include <SDL.h>
#include <SDL_image.h>
#include <Box2D/Box2D.h>
#include <tinyxml2.h>
#include "ae.h"
#include "vertex.h"
#include "world.h"
#include "level.h"

using namespace std;
using namespace tinyxml2;


/**
	Ctor.
*/
Level::Level()
{
	m_vbo = 0;
	m_tex = 0;
}


/**
	Dtor.
*/
Level::~Level()
{
}



/**
	Initializes a level for use. Call fini() for clean up.

	\return		1 for success, 0 otherwise
*/
int	Level::init()
{
	//
	// Create and prepare VBO.
	//
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(Vertex) * m_tile_coords.size(),
		&m_tile_coords[0]._x,
		GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));

	//
	// Textures.
	//
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	SDL_Surface *teximage = IMG_Load(m_image_file_name.c_str());
	if (!teximage)
		return 0;
	gluBuild2DMipmaps(
		GL_TEXTURE_2D,		// texture to specify
		GL_RGBA,			// internal texture storage format
		teximage->w,		// texture width
		teximage->h,		// texture height
		GL_RGBA,			// pixel format (possibly RGBA)
		GL_UNSIGNED_BYTE,	// color component format
		teximage->pixels	// pointer to texture image
		);
	SDL_FreeSurface(teximage);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glDisable(GL_TEXTURE_2D);
	return (glGetError() == GL_NO_ERROR);
}


/**
	Render level.
*/
void Level::render()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glDrawArrays(GL_QUADS, 0, m_tile_coords.size());
	glDisable(GL_TEXTURE_2D);
}


/**
	Level clean up.
*/
void Level::fini()
{
	glDeleteBuffers(1, &m_vbo);
	m_vbo = 0;

	glDeleteTextures(1, &m_tex);
	m_tex = 0;
}


/**
	Load level data from a file.

	\param filename		input file name
	\return				1 for success, 0 otherwise
*/
int Level::load(const char *filename)
{
	//
	// Load and parse XML description for a level.
	//
	tinyxml2::XMLDocument doc;
    XMLError ok = doc.LoadFile(filename);
	if (ok != XML_SUCCESS)
		return 0;
	XMLElement *root = doc.FirstChildElement();

	// Data to extract from the level XML.
	int		tile_width;
	int		tile_height;
	int		image_file_width;
	int		image_file_height;
	int		level_width;
	int		level_height;

	// Tile properties.
	XMLElement *tileset_element;
	tileset_element		= root->FirstChildElement();
	tile_width			= tileset_element->IntAttribute("tilewidth");
	tile_height			= tileset_element->IntAttribute("tileheight");

	// Image file properties.
	XMLElement *image_element;
	image_element		= root->FirstChildElement()->FirstChildElement();
	m_image_file_name	= image_element->Attribute("source");
	image_file_width	= image_element->IntAttribute("width");
	image_file_height	= image_element->IntAttribute("height");

	// Level properties.
	XMLElement *layer_element;
	layer_element		= root->FirstChildElement()->NextSiblingElement();
	level_width			= layer_element->IntAttribute("width");
	level_height		= layer_element->IntAttribute("height");

	// First read tile "gids" in as such.
	vector<int> tile_gids;
	XMLElement *tile_element
		= layer_element->FirstChildElement()->FirstChildElement();
	while (tile_element)
	{
		tile_gids.push_back(tile_element->IntAttribute("gid"));
		tile_element = tile_element->NextSiblingElement();
	}

	// Convert non-empty gids into tile coordinates.
	for (int y = level_height - 1, i = 0; y >= 0; --y)
		for (int x = 0; x < level_width; ++x, i++)
		
		{
			if (tile_gids[i] == 0)
				continue;

			// Calculate coordinates based on x, y position and tile widths.
			GLint top_left_x	= x * tile_width;
			GLint top_left_y	= (y + 1) * tile_height;
			GLint top_right_x	= (x + 1) * tile_width;
			GLint top_right_y	= (y + 1) * tile_height;
			GLint bot_right_x	= (x + 1) * tile_width;
			GLint bot_right_y	= y * tile_height;
			GLint bot_left_x	= x * tile_width;
			GLint bot_left_y	= y * tile_height;

			// Calculate texture coordinates.
			int tex_per_row = image_file_width / tile_width;
			int tex_per_col = image_file_height / tile_height;
			int row = tile_gids[i] / (tex_per_row + 1);
			int col = (tile_gids[i] - 1) % (image_file_width / tile_width);
			GLfloat bot_left_s = (float)(col * tile_width) / (float)image_file_width;
			GLfloat bot_left_t = (float)((row + 1) * tile_height - 1) / (float)image_file_height;
			GLfloat bot_right_s = (float)((col + 1) * tile_width - 1) / (float)image_file_width;
			GLfloat bot_right_t = bot_left_t;
			GLfloat top_right_s = bot_right_s;
			GLfloat top_right_t = (float)(row * tile_height) / (float)image_file_height;
			GLfloat top_left_s = bot_left_s;
			GLfloat top_left_t = top_right_t;

			// Create the vertexes and push them to the vertex array.
			Vertex v0, v1, v2, v3;
			v0._x = top_left_x;	v1._x = top_right_x;	v2._x = bot_right_x;	v3._x = bot_left_x;
			v0._y = top_left_y;	v1._y = top_right_y;	v2._y = bot_right_y;	v3._y = bot_left_y;
			v0._z = 0;			v1._z = 0;				v2._z = 0;				v3._z = 0;
			v0._s = top_left_s;	v1._s = top_right_s;	v2._s = bot_right_s;	v3._s = bot_left_s;
			v0._t = top_left_t;	v1._t = top_right_t;	v2._t = bot_right_t;	v3._t = bot_left_t;
			m_tile_coords.push_back(v0);
			m_tile_coords.push_back(v1);
			m_tile_coords.push_back(v2);
			m_tile_coords.push_back(v3);

			// Create static bodies for physics.
			// No body for ornaments. 
			/// TODO: remove hardcoding.
			if (tile_gids[i] == 2 || tile_gids[i] == 3)
				continue;
			float width = (top_right_x - top_left_x) / (float)PHYS_SCALE;
			float height = (top_left_y - bot_left_y) / (float)PHYS_SCALE;
			float half_width = (float)width / 2.0;
			float half_height = (float)height / 2.0;
			float middle_x = top_left_x / (float)PHYS_SCALE + half_width;
			float middle_y = bot_left_y / (float)PHYS_SCALE + half_height;
			b2BodyDef groundBodyDef;
			groundBodyDef.position.Set(middle_x, middle_y);
			b2Body* groundBody = World::get()->CreateBody(&groundBodyDef);
			b2PolygonShape groundBox;
			groundBox.SetAsBox(half_width, half_height);
			groundBody->CreateFixture(&groundBox, 0.0f);
		}

	return 1;
}
