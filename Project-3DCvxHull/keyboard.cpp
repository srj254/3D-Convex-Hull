#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>

/**
*	Function invoked when an event on regular keys occur
*/
void keyboard(unsigned char k, int x, int y)
{
	/* Show which key was pressed */
	std::cout << "Pressed \"" << k << "\" ASCII: " << (int)k << std::endl;
	/* Close application if ESC is pressed */
	if (k == 27)
	{
		exit(0);
	}
}
