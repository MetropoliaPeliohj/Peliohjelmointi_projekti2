#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

/**
	Base class for all objects that can be rendered onto screen.
*/
class Renderable
{
public:
	Renderable()			{}
	virtual ~Renderable()	{}

	virtual void render() = 0;
};

#endif // _RENDERABLE_H_
