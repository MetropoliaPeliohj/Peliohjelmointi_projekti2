#include <algorithm>
#include "world.h"
#include "deletion_list.h"

Deletion_List* Deletion_List::m_instance = 0;


/**
	Adds an item to the list (only once).
*/
void Deletion_List::push_back_unique(Physable *p)
{
	if (std::find(begin(), end(), p) == end())
		push_back(p);
}


/**
	Removes all the items from the World and deletes them.
	Clears the list.
*/
void Deletion_List::delete_and_clear()
{
	for (Deletion_List::iterator i = begin(); i != end(); i++)
	{
		World::get()->DestroyBody((*i)->get_body());
 		delete (*i);
	}
	clear();
}
