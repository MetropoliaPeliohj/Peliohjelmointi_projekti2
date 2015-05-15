#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "render_list.h"

Render_List* Render_List::m_instance = 0;


/**
	Renders list items in order. Set modelview matrix beforehand.
*/
void Render_List::render_all()
{
	for (Render_List::iterator i = begin(); i != end(); i++)
	{
		glPushMatrix();
		(*i)->render();
		glPopMatrix();
	}
}
