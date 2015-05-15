#ifndef _AI_CHARACTER_H_
#define _AI_CHARACTER_H_

/**
	Base class for AI controlled objects.
*/
class AI_Character
{
public:
	AI_Character()			{}
	virtual ~AI_Character()	{}

	virtual void run_ai() = 0;
};

#endif // _AI_CHARACTER_H_
