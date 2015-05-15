#include <windows.h>
#include <math.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include <SDL.h>
#include "ae.h"
#include "world.h"
#include "duck.h"

GLuint Duck::m_dl		= 0;
Duck*  Duck::m_instance = 0;


/**
	(Singleton) instance access.
*/
Duck* Duck::get_duck()
{
	return Duck::m_instance;
}


/**
	Ctor.
*/
Duck::Duck(int x, int y) : _x(x), _y(y)
{
	// Dynamic body for physics.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(
		x / (float)PHYS_SCALE,
		y / (float)PHYS_SCALE);
	m_body = World::get()->CreateBody(&bodyDef);
	b2CircleShape circleShape;
	circleShape.m_radius = DUCK_RADIUS / (float)PHYS_SCALE;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = DUCK_DENSITY;
	fixtureDef.friction = DUCK_FRICTION;
	m_body->CreateFixture(&fixtureDef);
	m_body->SetUserData(this);

	// Set (singleton) instance.
	Duck::m_instance = this;
}


/**
	Dtor.
*/
Duck::~Duck()
{
}


/**
	Initializes duck for rendering.

	Returns 1 for success, 0 otherwise.
*/
int	Duck::init()
{
	if (Duck::m_dl)
		return 1;

	//
	// Prepare a display list for rendering.
	// TODO: NOW JUST A PLACEHOLDER (CIRCLE).
	//
	Duck::m_dl = glGenLists(1);
	glNewList(Duck::m_dl, GL_COMPILE);
		glBegin(GL_LINE_LOOP);
			for (int i=0; i < 360; i++)
			{
				glVertex3f(
					cos(DEG2RAD(i)) * DUCK_RADIUS,
					sin(DEG2RAD(i)) * DUCK_RADIUS,
					0.0);
			}
			glVertex3f(0.0, 0.0, 0.0);
		glEnd();
	glEndList();
	return (glGetError() == GL_NO_ERROR);
}


/**
	Render duck.
*/
void Duck::render()
{
	glMatrixMode(GL_MODELVIEW);
	b2Vec2 pos = m_body->GetPosition();
	float angle = RAD2DEG(m_body->GetAngle());
	glTranslatef(pos.x * PHYS_SCALE, pos.y * PHYS_SCALE, 1.0);
	glRotatef(angle, 0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	glCallList(m_dl);
}


/**
	Rendering clean-up.
*/
void Duck::fini()
{
	glDeleteLists(Duck::m_dl, 1);
	Duck::m_dl = 0;
}


/**
	Apply force to move the duck left.
*/
void Duck::left()
{
	m_body->ApplyForceToCenter(b2Vec2(DUCK_FORCE_LEFT, 0));
}


/**
	Apply force to move the duck right.
*/
void Duck::right()
{
	m_body->ApplyForceToCenter(b2Vec2(DUCK_FORCE_RIGHT, 0));
}


/**
	Apply force to make the duck jump/fly.
*/
void Duck::jump()
{
	m_body->ApplyForceToCenter(b2Vec2(0, DUCK_FORCE_JUMP));
}


/**
	Set modelview matrix so that the duck is centralized
	and scaling applied.
*/
void Duck::set_camera_to_duck() const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//
	// Change scaling according to height. A better idea would be
	// to change scaling according to velocity.
	//
	// For constant scale, set scaling = GF_SCALE.
	//
	b2Vec2 pos = m_body->GetPosition();
	float scaling = (DUCK_ORIG_Y - pos.y * PHYS_SCALE) / DUCK_ORIG_Y;
	if (scaling < 0.1)
		scaling = 0.1;
	if (scaling > 1.0)
		scaling = 1.0;

	//
	// Center the duck.
	//
	gluLookAt(
		pos.x * PHYS_SCALE * scaling - WIDTH / 2,
		pos.y * PHYS_SCALE * scaling - HEIGHT / 2,
		1,
		pos.x * PHYS_SCALE * scaling - WIDTH / 2,
		pos.y * PHYS_SCALE * scaling - HEIGHT / 2,
		0,
		0,
		1,
		0
		);

	glScalef(scaling, scaling, 1);
}
