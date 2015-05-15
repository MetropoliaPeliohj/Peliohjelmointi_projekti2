#include <windows.h>
#include <math.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include <SDL.h>
#include <SDL_image.h>
#include <Box2D/Box2D.h>
#include "ae.h"
#include "world.h"
#include "hunter.h"

GLuint Hunter::m_dl = 0;
GLuint Hunter::m_tex = 0;


/**
	Ctor.

	\param	x		initial x-coordinate
	\param	y		initial y-coordinate
	\param min_x	min x-coordinate
	\param max_x	max x-coordinate
*/
/*
Ctor.
*/
Hunter::Hunter(int x, int y, int min_x, int max_x)
{
	m_min_x = min_x;
	m_max_x = max_x;

	// Dynamic body for physics.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(
		x / (float)PHYS_SCALE,
		y / (float)PHYS_SCALE);
	m_body = World::get()->CreateBody(&bodyDef);
	b2PolygonShape box;
	box.SetAsBox(
		(HUNTER_WIDTH / 2.0) / (float)PHYS_SCALE,
		(HUNTER_HEIGHT / 2.0) / (float)PHYS_SCALE);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.density = HUNTER_DENSITY;
	fixtureDef.friction = HUNTER_FRICTION;
	m_body->CreateFixture(&fixtureDef);
	m_body->SetUserData(this);

	// Set inactive so it cannot collide with bullets etc.
	/// TODO: reconsider this
	m_body->SetActive(false);

	// Create decision tree for hunter AI.
	Decide_Shooting		*ds = new Decide_Shooting(this);
	Decide_Moving		*dm = new Decide_Moving(this);
	Is_Duck_On_Left		*dl = new Is_Duck_On_Left(this);
	Is_Duck_On_Right	*dr = new Is_Duck_On_Right(this);
	Do_Shoot			*s = new Do_Shoot(this);
	Do_Move_Left		*l = new Do_Move_Left(this);
	Do_Move_Right		*r = new Do_Move_Right(this);
	Do_Wait				*w1 = new Do_Wait(this);
	Do_Wait				*w2 = new Do_Wait(this);
	ds->_child[0] = s;	ds->_child[1] = dm;
	dm->_child[0] = dl; dm->_child[1] = w1;
	dl->_child[0] = l;	dl->_child[1] = dr;
	dr->_child[0] = r;	dr->_child[1] = w2;
	m_decision_tree = ds;
}


/**
	Dtor.
*/
Hunter::~Hunter()
{
}


/**
	Renderable initialization.

	Returns 1 for success, 0 otherwise.
*/
int	Hunter::init_rendering()
{
	if (Hunter::m_dl)
		return 1;

	//
	// Prepare a display list for rendering.
	// TODO: NOW JUST A PLACEHOLDER (CROSSED BOX).
	//
	m_dl = glGenLists(1);
	glNewList(m_dl, GL_COMPILE);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 0.0); glVertex3f(-HUNTER_WIDTH / 2.0, HUNTER_HEIGHT / 2.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(HUNTER_WIDTH / 2.0, HUNTER_HEIGHT / 2.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(HUNTER_WIDTH / 2.0, -HUNTER_HEIGHT / 2.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-HUNTER_WIDTH / 2.0, -HUNTER_HEIGHT / 2.0, 0.0);
	glEnd();
	glEndList();

	//
	// Textures.
	//
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	SDL_Surface *teximage = IMG_Load("chrome.png");
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
void Hunter::render()
{
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	b2Vec2 pos = m_body->GetPosition();
	float angle = m_body->GetAngle() * 57.2957795;
	glTranslatef(pos.x * PHYS_SCALE, pos.y * PHYS_SCALE, 1.0);
	glRotatef(angle, 0.0, 0.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glCallList(m_dl);
	glDisable(GL_TEXTURE_2D);
}


/**
	Renderable finalization.
*/
void Hunter::finish_rendering()
{
	glDeleteLists(Hunter::m_dl, 1);
	Hunter::m_dl = 0;

	glDeleteTextures(1, &m_tex);
	m_tex = 0;
}


/**
	Hunter AI execution.
*/
void Hunter::run_ai()
{
	m_decision_tree->evaluate();
}
