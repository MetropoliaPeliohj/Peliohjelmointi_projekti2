#include "ai_list.h"

AI_List* AI_List::m_instance = 0;


/**
	Runs AI on all the characters in the list.
*/
void AI_List::run_ai_for_all()
{
	for (AI_List::iterator i = begin(); i != end(); i++)
	{
		(*i)->run_ai();
	}
}
