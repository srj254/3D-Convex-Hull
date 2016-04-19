#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "gen_codes.h"
#include <iostream>

extern int fchange;
extern int pchange;
/** Function invoked when a mouse event occur */
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

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