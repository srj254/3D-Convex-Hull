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

/**
*	Function invoked when an event on regular keys occur
*/
void keyboard(unsigned char k, int x, int y)
{
	/* Show which key was pressed */
	if (k == 'n' && state_index < (int)states.v_stateObjects.size() - 1 &&
		state_index != -1)
	{
		state_index++;
	}
	else if (k == 'p' && state_index > -1)
	{
		state_index--;
	}

	/* Close application if ESC is pressed */
	if (k == 27)
	{
		exit(0);
	}
}
