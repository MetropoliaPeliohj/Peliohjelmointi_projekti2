#ifndef _RENDER_LIST_H_
#define _RENDER_LIST_H_

#include <list>
#include "renderable.h"


/**
	For keeping track of and for rendering of
	all the renderable items. Singleton.
*/
class Render_List : public std::list<Renderable*>
{
public:

	// Instance access.
	static Render_List* get()
	{
		if (!Render_List::m_instance)
			Render_List::m_instance = new Render_List();
		return Render_List::m_instance;
	}
	
	// Render all items.
	void render_all();

private:

	Render_List()	{}
	~Render_List()	{}

	// (Singleton) instance;
	static Render_List* m_instance;
};

#endif // _RENDER_LIST_H_
