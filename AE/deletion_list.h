#ifndef _DELETION_LIST_H_
#define _DELETION_LIST_H_

#include <list>
#include "physable.h"


/**
	For collecting and deleting physics objects (e.g. Bullets).
*/
class Deletion_List : public std::list<Physable*>
{
public:

	// Instance access.
	static Deletion_List* get()
	{
		if (!Deletion_List::m_instance)
			Deletion_List::m_instance = new Deletion_List();
		return Deletion_List::m_instance;
	}

	void push_back_unique(Physable*);
	void delete_and_clear();

private:

	Deletion_List()		{}
	~Deletion_List()	{}

	// (Singleton) instance.
	static Deletion_List* m_instance;
};


#endif // _AI_BULLET_LIST_H_
