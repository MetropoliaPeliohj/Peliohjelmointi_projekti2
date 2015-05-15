#ifndef _WORLD_H_
#define _WORLD_H_

#include <Box2D/Box2D.h>
#include "ae.h"


/**
	Physics world (singleton) derived from b2World.
*/
class World : public b2World
{
public:

	// Instance access.
	static World* get()
	{
		if (!World::m_instance)
			World::m_instance = new World();
		return World::m_instance;
	}

private:

	World() : b2World(b2Vec2(0.0f, GRAVITY), true)	{}
	~World()										{}

	// Singleton instance of the class.
	static World* m_instance;
};

#endif // _WORLD_H_
