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

/** Function invoked when a mouse event occur */
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (b_rotate)
		{
			b_rotate = !b_rotate;
		}
		else if (state_index != -1 &&
				state_index < (int)states.v_stateObjects.size() - 1)
		{
			state_index++;
		}
	}
	return;
}

void passiveMotion(int x, int y)
{
	mouse_curX = x;
	mouse_curY = y;

	return;
}


void activeMotion(int x, int y)
{
	double X2 = (x - (windowWidth / 2)) *(x - (windowWidth / 2));
	double Y2 = (y - (windowHeight / 2))*(y - (windowHeight / 2));
	double dist2 = X2 + Y2;
	double s_radius2 = ((windowHeight / 2)* (windowHeight / 2));

	double newx = (x - (windowWidth / 2)) / (sqrt(dist2));
	double newy = (y - (windowHeight / 2)) / (sqrt(dist2));

	if (dist2 >= s_radius2)
	{
		v_x = newx;
		v_y = newy;
		v_z = 0.0;
		//cout << "X: " << newx << "Y: " << newy << "Z: " << 0.0 << endl;
	}
	else
	{
		double newz = sqrt((s_radius2 - dist2) / s_radius2);
		v_x = newx;
		v_y = newy;
		v_z = newz;
		//cout << "X: " << newx << "Y: " << newy << "Z: " << newz << endl;
	}

	return;
}