#ifndef _AI_LIST_H_
#define _AI_LIST_H_

#include <list>
#include "ai_character.h"


/**
	For keeping track of and running AI of
	all the AI characters. Singleton.
*/
class AI_List : public std::list<AI_Character*>
{
public:

	// Instance access.
	static AI_List* get()
	{
		if (!AI_List::m_instance)
			AI_List::m_instance = new AI_List();
		return AI_List::m_instance;
	}

	// Run AI on all list items.
	void run_ai_for_all();

private:

	AI_List()	{}
	~AI_List()	{}

	// (Singleton) instance.
	static AI_List* m_instance;
};


#endif // _AI_LIST_H_

