#include <math.h>
#include <stdlib.h>
#include "ae.h"
#include "bullet.h"
#include "duck.h"
#include "hunter.h"
#include "hunter_ai.h"
#include "render_list.h"


/**
	Decide if hunter wants to shoot.
	Probabilistic, depends on the distance to the duck.
*/
void Decide_Shooting::evaluate()
{
	float duck_x	= Duck::get_duck()->get_body()->GetPosition().x;
	float duck_y	= Duck::get_duck()->get_body()->GetPosition().y;
	float hunter_x	= m_hunter->get_body()->GetPosition().x;
	float hunter_y	= m_hunter->get_body()->GetPosition().y;
	float distance	= PHYS_SCALE *
		sqrt(pow(duck_x - hunter_x, 2) + pow(duck_y - hunter_y, 2));

	// Not shooting at all from too far away.
	if (distance > HUNTER_SHOOT_DSTNC)
		_child[1]->evaluate();

	// Randomize shooting, the nearer the more probably shoot.
	if (
		rand() % HUNTER_SHOOT_DSTNC > distance && 
		rand() / (float)RAND_MAX <= HUNTER_SHOOTING_P
	)
		_child[0]->evaluate();
	else
		_child[1]->evaluate();
}


/**
	Decide if hunter wants to move at all. Probabilistic.
*/
void Decide_Moving::evaluate()
{
	if (rand() / (float)RAND_MAX <= HUNTER_MOVING_P)
		_child[0]->evaluate();
	else
		_child[1]->evaluate();
}


/**
	Check if the duck is on left.
*/
void Is_Duck_On_Left::evaluate()
{
	float duck_x = Duck::get_duck()->get_body()->GetPosition().x;
	float hunter_x = m_hunter->get_body()->GetPosition().x;

	if (duck_x < hunter_x)
		_child[0]->evaluate();
	else
		_child[1]->evaluate();
}


/**
	Check if the duck is on right.
*/
void Is_Duck_On_Right::evaluate()
{
	float duck_x = Duck::get_duck()->get_body()->GetPosition().x;
	float hunter_x = m_hunter->get_body()->GetPosition().x;

	if (duck_x > hunter_x)
		_child[0]->evaluate();
	else
		_child[1]->evaluate();
}


/**
	Shoot.
*/
void Do_Shoot::evaluate()
{
	float duck_x	= Duck::get_duck()->get_body()->GetPosition().x;
	float duck_y	= Duck::get_duck()->get_body()->GetPosition().y;
	float hunter_x	= m_hunter->get_body()->GetPosition().x;
	float hunter_y	= m_hunter->get_body()->GetPosition().y;

	//
	// Create a new bullet aimed at the duck.
	// Add to the render list.
	//
	Bullet* bullet = new Bullet(
		hunter_x * PHYS_SCALE,
		hunter_y * PHYS_SCALE,
		RAD2DEG(atan2(duck_y - hunter_y, duck_x - hunter_x)),
		BULLET_FORCE
	);
	Render_List::get()->push_back(bullet);
}


/**
	Move to left.
*/
void Do_Move_Left::evaluate()
{
	float hunter_x = m_hunter->get_body()->GetPosition().x;

	// Check if the hunter is allowed to move left.
	if (hunter_x * PHYS_SCALE <= m_hunter->get_min_x())
		return;

	// Move the hunter body left.
	m_hunter->get_body()->SetTransform(
		b2Vec2(
			hunter_x - HUNTER_SPEED / (float)PHYS_SCALE,
			m_hunter->get_body()->GetPosition().y
		),
		m_hunter->get_body()->GetAngle()
	);
}


/**
	Move to right.
*/
void Do_Move_Right::evaluate()
{
	float hunter_x = m_hunter->get_body()->GetPosition().x;

	// Check if the hunter is allowed to move left.
	if (hunter_x * PHYS_SCALE >= m_hunter->get_max_x())
		return;

	// Move the hunter body left.
	m_hunter->get_body()->SetTransform(
		b2Vec2(
			hunter_x + HUNTER_SPEED / (float)PHYS_SCALE,
			m_hunter->get_body()->GetPosition().y
		),
		m_hunter->get_body()->GetAngle()
	);
}


/*
	Wait (do nothing).
*/
void Do_Wait::evaluate()
{
	return;
}
