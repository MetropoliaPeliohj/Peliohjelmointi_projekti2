#include <windows.h>
#include <math.h>
#include <gl/glew.h>
#include <gl/GL.h>
#include <SDL.h>
#include <Box2D/Box2D.h>
#include "ae.h"
#include "world.h"
#include "bullet.h"

GLuint Bullet::m_dl = 0;


/*
	Ctor.
*/
Bullet::Bullet(int x, int y, float angle, float force)
{
	// Dynamic body for physics.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(
		x / (float)PHYS_SCALE, 
		y / (float)PHYS_SCALE);

	// Set as a bullet for continuous collision detection.
	bodyDef.bullet = true;

	m_body = World::get()->CreateBody(&bodyDef);
	b2CircleShape circleShape;
	circleShape.m_radius = BULLET_RADIUS / (float)PHYS_SCALE;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = BULLET_DENSITY;
	fixtureDef.friction = BULLET_FRICTION;
	m_body->CreateFixture(&fixtureDef);
	m_body->SetUserData(this);

	// Fire the bullet!
	m_body->ApplyForceToCenter(
		b2Vec2(
			force * cos(DEG2RAD(angle)), 
			force * sin(DEG2RAD(angle))
		)
	);
}


/*
	Dtor.
*/
Bullet::~Bullet()
{
}


/*
	Renderable initialization.

	Returns 1 for success, 0 otherwise.
*/
int	Bullet::init_rendering()
{
	if (Bullet::m_dl)
		return 1;

	//
	// Prepare a display list for rendering.
	//
	Bullet::m_dl = glGenLists(1);
	glNewList(Bullet::m_dl, GL_COMPILE);
		glBegin(GL_LINE_LOOP);
			for (int i=0; i < 360; i += 4)
			{
				glVertex3f(
					cos(DEG2RAD(i)) * BULLET_RADIUS,
					sin(DEG2RAD(i)) * BULLET_RADIUS,
					0.0);
			}
		glEnd();
	glEndList();
	return (glGetError() == GL_NO_ERROR);
}


/*
	Render Bullet.
*/
void Bullet::render()
{
	glMatrixMode(GL_MODELVIEW);
	b2Vec2 pos = m_body->GetPosition();
	float angle = m_body->GetAngle() * 57.2957795;
	glTranslatef(pos.x * PHYS_SCALE, pos.y * PHYS_SCALE, 0.5);
	glRotatef(angle, 0.0, 0.0, 1.0);
	glCallList(m_dl);
}


/*
	Renderable finalization.
*/
void Bullet::finish_rendering()
{
	glDeleteLists(m_dl, 1);
	Bullet::m_dl = 0;
}
