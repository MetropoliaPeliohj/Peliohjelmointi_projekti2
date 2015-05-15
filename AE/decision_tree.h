#ifndef _DEC_TREE_H_
#define _DEC_TREE_H_

#include <iostream>


/*
	Abstract decision tree node.
	Inherit and implement your own sub-classes!
*/
class Decision_Tree_Node
{
public:
	Decision_Tree_Node()
	{
		_child[0] = 0;
		_child[1] = 0;
	}

	virtual ~Decision_Tree_Node()
	{
		delete _child[0];
		delete _child[1]; 
	}

	// Evaluate node: either proceed
	// to evaluate a child node, or
	// execute an action (leaf node).
	// Implement this in your sub-classes!
	virtual void evaluate() = 0;

	// Binary tree! (public for convenience...).
	Decision_Tree_Node* _child[2];
};


#endif // _DEC_TREE_NODE_H_
