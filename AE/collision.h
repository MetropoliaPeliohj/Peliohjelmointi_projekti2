#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <Box2D/Box2D.h>


/**
	Collision handler (singleton).
*/
class Contact_Listener : public b2ContactListener
{
public:

	// Instance access.
	static Contact_Listener* get()
	{
		if (!Contact_Listener::m_instance)
			Contact_Listener::m_instance = new Contact_Listener();
		return Contact_Listener::m_instance;
	}

	// Contact callbacks.
	void BeginContact(b2Contact*);
	void EndContact(b2Contact*);

private:

	Contact_Listener()				{}
	~Contact_Listener()				{}

	// (Singleton) instance.
	static Contact_Listener* m_instance;
};

#endif
