#include "ae.h"
#include "bullet.h"
#include "deletion_list.h"
#include "duck.h"
#include "log.h"
#include "render_list.h"
#include "collision.h"

Contact_Listener* Contact_Listener::m_instance = 0;


/*
	Collision (begin).
*/
void Contact_Listener::BeginContact(b2Contact *contact)
{
	Log::log(LOG_INFO, "Begin contact");
	b2Body *body_a = contact->GetFixtureA()->GetBody();
	b2Body *body_b = contact->GetFixtureB()->GetBody();
	b2Body *body_d = Duck::get_duck()->get_body();

	// Check if the duck bites a bullet.
	if (body_a->IsBullet() || body_b->IsBullet())
	{
		if (body_a == body_d || body_b == body_d)
		{
			Duck::get_duck()->kill();
		}
	}

	// Delete bullets from rendering and physics.
	if (body_a->IsBullet())
	{
		Render_List::get()->remove((Bullet*)(body_a->GetUserData()));
		Deletion_List::get()->push_back_unique((Bullet*)(body_a->GetUserData()));
	}
	if (body_b->IsBullet())
	{
		Render_List::get()->remove((Bullet*)(body_b->GetUserData()));
		Deletion_List::get()->push_back_unique((Bullet*)(body_b->GetUserData()));
	}
}


/**
	Collision (end).
*/
void Contact_Listener::EndContact(b2Contact *contact)
{
	Log::log(LOG_INFO, "End contact");
}
