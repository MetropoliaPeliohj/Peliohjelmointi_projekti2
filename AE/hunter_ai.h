#ifndef _HUNTER_AI_H_
#define _HUNTER_AI_H_

#include "decision_tree.h"
class Hunter;


/**
	Classes for implementing hunter's decision tree.
	See hunter_ai_tree.pptx in resource files.
*/
class Hunter_AI_Node : public Decision_Tree_Node
{
public:
	Hunter_AI_Node(Hunter *h)	{ m_hunter = h; }
	virtual ~Hunter_AI_Node()	{}

protected:
	Hunter* m_hunter;
};


class Decide_Shooting : public Hunter_AI_Node
{
public:
	Decide_Shooting(Hunter *h) : Hunter_AI_Node(h)	{}
	virtual ~Decide_Shooting()						{}
	virtual void evaluate();
};


class Decide_Moving : public Hunter_AI_Node
{
public:
	Decide_Moving(Hunter *h) : Hunter_AI_Node(h)	{}
	virtual ~Decide_Moving()						{}
	virtual void evaluate();
};


class Is_Duck_On_Left : public Hunter_AI_Node
{
public:
	Is_Duck_On_Left(Hunter *h) : Hunter_AI_Node(h)	{}
	virtual ~Is_Duck_On_Left()						{}
	virtual void evaluate();
};


class Is_Duck_On_Right : public Hunter_AI_Node
{
public:
	Is_Duck_On_Right(Hunter *h) : Hunter_AI_Node(h)	{}
	virtual ~Is_Duck_On_Right()						{}
	virtual void evaluate();
};


class Do_Shoot : public Hunter_AI_Node
{
public:
	Do_Shoot(Hunter *h) : Hunter_AI_Node(h)			{}
	virtual ~Do_Shoot()								{}
	virtual void evaluate();
};


class Do_Move_Left : public Hunter_AI_Node
{
public:
	Do_Move_Left(Hunter *h) : Hunter_AI_Node(h)		{}
	virtual ~Do_Move_Left()							{}
	virtual void evaluate();
};


class Do_Move_Right : public Hunter_AI_Node
{
public:
	Do_Move_Right(Hunter *h) : Hunter_AI_Node(h)	{}
	virtual ~Do_Move_Right()						{}
	virtual void evaluate();
};


class Do_Wait : public Hunter_AI_Node
{
public:
	Do_Wait(Hunter *h) : Hunter_AI_Node(h)			{}
	virtual ~Do_Wait()								{}
	virtual void evaluate();
};


#endif // _HUNTER_AI_H_
