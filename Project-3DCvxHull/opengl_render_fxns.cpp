#include <iostream>
#include <iterator>
#include <vector>
#include <set>

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glui\glui.h"

#include "gen_codes.h"
#include "Point.h"
#include "Facet.h"
#include "Halfedge.h"
#include "ConflictGraph.h"
#include "menu.h"

using namespace std;

/** Window reshape function */
void changeSize(int w, int h)
{
	return;
}

float triColor[] = {
	1.0, 0.0, 0.0, 1.0,	// Right - Top - Front		(White)
};
//	0.0, 1.0, 0.0, 1.0, // Right - Top - Back		(Yellow)
//	0.0, 0.0, 1.0, 1.0,	// Right - Top - Front		(White)
//	1.0, 0.0, 0.0, 1.0,	// Right - Top - Front		(White)
//};
//0.0, 1.0, 0.0, 1.0, // Right - Top - Back		(Yellow)
//	0.0, 0.0, 1.0, 1.0,	// Right - Top - Front		(White)
//	1.0, 0.0, 0.0, 1.0,	// Right - Top - Front		(White)
//	0.0, 1.0, 0.0, 1.0, // Right - Top - Back		(Yellow)
//	0.0, 0.0, 1.0, 1.0,	// Right - Top - Front		(White)
//	1.0, 0.0, 0.0, 1.0,	// Right - Top - Front		(White)
//	0.0, 1.0, 0.0, 1.0, // Right - Top - Back		(Yellow)
//	0.0, 0.0, 1.0, 1.0,	// Right - Top - Front		(White)
//	1.0, 0.0, 0.0, 1.0,	// Right - Top - Front		(White)
//	0.0, 1.0, 0.0, 1.0, // Right - Top - Back		(Yellow)
//	0.0, 0.0, 1.0, 1.0,	// Right - Top - Front		(White)
//	1.0, 0.0, 0.0, 1.0,	// Right - Top - Front		(White)
//	0.0, 1.0, 0.0, 1.0, // Right - Top - Back		(Yellow)
//	0.0, 0.0, 1.0, 1.0,	// Right - Top - Front		(White)
//};

float linecolor[] = { 
1.0, 0.0, 0.0, 0.5,
1.0, 0.0, 0.0, 0.5,
0.0, 1.0, 0.0, 0.5, 0.0, 1.0, 0.0, 0.5,
0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };

float mytri[4] = { 1.0, 0.0, 0.0, 0.5 };
/** Function invoked for drawing using OpenGL */
void display()
{
	/* Clear the window */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Load the projection matrix and
	   set the perspective projection values */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, +1, -1, +1, 10, 100.0);

	/* Load the model view matrix */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Draw the cube */
	glTranslatef(0, 0, -20);
	glPointSize(10);

	if (b_rotate)
	{
		glRotatef(angle, 1.0, 0.5, 0.0);
		angle += (float)0.05;
	}
	else
		glRotatef(angle, 0.8, -0.5, 0.5);

	
	{
		getPtsInArray(); // Get the points
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		{
			if (nPts > 0)
			{
				glVertexPointer(3, GL_FLOAT, 0, pointArray);
				glColorPointer(4, GL_FLOAT, 0, linecolor);
				glDrawElements(GL_POINTS, nPts, GL_UNSIGNED_BYTE, ptIndex);
			}
		}
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	
	// Get the Faces and lines
	{
		getFacesInArray();
		if (nFaces > 0)
		{
			if (!b_wireframe)
			{
				for (int i = 0; i < nFaces-5; i++)
				{
					/* Enable clients */
					glEnableClientState(GL_VERTEX_ARRAY);
					glEnableClientState(GL_COLOR_ARRAY);

					glVertexPointer(3, GL_FLOAT, 0, pointArray);
					glColorPointer(4, GL_FLOAT, 0, triColor);
					glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE,
									hullFaces + (3*i));
					glDisableClientState(GL_COLOR_ARRAY);
					glDisableClientState(GL_VERTEX_ARRAY);
					
				}
			}
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, pointArray);
			glColorPointer(4, GL_FLOAT, 0, linecolor);
			glDrawElements(GL_LINES, 2 * nHEdges, GL_UNSIGNED_BYTE, hullEdges);

			/* Disable clients */
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);

		}

	}


	glFlush();

	/* Swap buffers for animation */
	glutSwapBuffers();
}

/** Function invoked when window system events are not being received */
void idle()
{
	/* Redraw the window */
	if (glutGetWindow() != windowID)
	{
		glutSetWindow(windowID);
	}
	glutSetWindow(windowID);
	glutPostRedisplay();
}

/** Set OpenGL initial state */
void init()
{
	/* Set clear color */
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClearDepth(1.0);
	/* Enable the depth buffer */
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
}
