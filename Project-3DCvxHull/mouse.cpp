#include <iostream>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "gen_codes.h"
#include "Point.h"
#include "Facet.h"
#include "Halfedge.h"
#include "ConflictGraph.h"
#include "StateObject.h"
#include "menu.h"


extern int fchange;
extern int pchange;
/** Function invoked when a mouse event occur */
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (b_rotate)
			b_rotate = !b_rotate;
		else if (state_index != -1 && 
				state_index < states.v_stateObjects.size()-1)
			state_index++;
	}
	return;
}

void passiveMotion(int x, int y)
{
	return;
}

void activeMotion(int x, int y)
{
	return;
}