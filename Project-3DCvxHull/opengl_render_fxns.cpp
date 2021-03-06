#include <iostream>
#include <iterator>
#include <vector>
#include <set>
#include <time.h>

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glui\glui.h"

#include "gen_codes.h"
#include "Point.h"
#include "Facet.h"
#include "Halfedge.h"
#include "ConflictGraph.h"
#include "StateObject.h"
#include "menu.h"


using namespace std;

/** Window reshape function */
void changeSize(int w, int h)
{
	return;
}

/** Function invoked for drawing using OpenGL */
void display()
{
	/* Clear the window */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Load the projection matrix and
	   set the perspective projection values */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, +1, -1, +1, 10, 100);

	/* Load the model view matrix */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Draw the cube */
	glTranslatef(0, 0, -30 + zoom);
	glPointSize(10);

	if (b_rotate)
	{
		int xx = X->get_int_val();
		int yy = Y->get_int_val();
		int zz = Z->get_int_val();
		if (0 == xx && 0 == yy && 0 == zz)
			zz = 1.0;
		glRotatef(angle, xx, 
						 yy, 
						 zz);
		angle += (float)0.05;
	}
	else
	{
		int xx = X->get_int_val();
		int yy = Y->get_int_val();
		int zz = Z->get_int_val();
		if (0 == xx && 0 == yy && 0 == zz)
			zz = 1.0;
		glRotatef(angle, xx,
			yy,
			zz);
	}
	//state_index = states.v_stateObjects.size()-5;
	if (state_index > -1)
	{
		StateObject S = states.v_stateObjects.at(state_index);

		getPtsInArray(); // Load the points into an array
		for (unsigned i = 0; i < S.polygon_faces.size() && !b_wireframe; i++)
		{
			tri_vertices[0] = S.polygon_faces.at(i).vertices[0];
			tri_vertices[1] = S.polygon_faces.at(i).vertices[1];
			tri_vertices[2] = S.polygon_faces.at(i).vertices[2];
			
			init_norm_face_color();

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, pointArray);
			glColorPointer(4, GL_FLOAT, 0, ptColors);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, tri_vertices);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			memset(tri_vertices, 0x00, 3 * sizeof(int));
			memset(ptColors, 0x00, 4 * sizeof(float));
		}
		// Newly added face
		if (S.has_new_face && !b_wireframe)
		{
			getPtsInArray(); // Load the points into an array
			tri_vertices[0] = S.new_face.vertices[0];
			tri_vertices[1] = S.new_face.vertices[1];
			tri_vertices[2] = S.new_face.vertices[2];

			init_new_face_color();

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, pointArray);
			glColorPointer(4, GL_FLOAT, 0, ptColors);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, tri_vertices);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			memset(tri_vertices, 0x00, 3 * sizeof(int));
			memset(ptColors, 0x00, 4 * sizeof(float));
		}

		for (unsigned i = 0; i < S.remove_faces.size() && !b_wireframe; i++)
		{
			tri_vertices[0] = S.remove_faces.at(i).vertices[0];
			tri_vertices[1] = S.remove_faces.at(i).vertices[1];
			tri_vertices[2] = S.remove_faces.at(i).vertices[2];

			init_remv_face_color();

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, pointArray);
			glColorPointer(4, GL_FLOAT, 0, ptColors);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, tri_vertices);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			memset(tri_vertices, 0x00, 3 * sizeof(int));
			memset(ptColors, 0x00, 4 * sizeof(float));
		}
		
		getPtsInArray(); // Load the points into an array
		for (unsigned i = 0; i < S.horizon_edges.size(); i++)
		{
			edg_vertices[0] = S.horizon_edges.at(i).vertices[0];
			edg_vertices[1] = S.horizon_edges.at(i).vertices[1];

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			init_horizon_line_color();

			glVertexPointer(3, GL_FLOAT, 0, pointArray);
			glColorPointer(4, GL_FLOAT, 0, ptColors);
			glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, edg_vertices);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			memset(edg_vertices, 0x00, 2 * sizeof(int));
			memset(ptColors, 0x00, 4 * sizeof(float));
		}

		getPtsInArray(); // Load the points into an array
		for (unsigned i = 0; i < S.edges.size(); i++)
		{
			edg_vertices[0] = S.edges.at(i).vertices[0];
			edg_vertices[1] = S.edges.at(i).vertices[1];
	
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			init_norm_line_color();

			glVertexPointer(3, GL_FLOAT, 0, pointArray);
			glColorPointer(4, GL_FLOAT, 0, ptColors);
			glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, edg_vertices);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			memset(edg_vertices, 0x00, 2 * sizeof(int));
			memset(ptColors, 0x00, 4 * sizeof(float));
		}
	}
	
	{
		getPtsInArray(); // Load the points into an array
		if (state_index > -1)
		{
			StateObject S = states.v_stateObjects.at(state_index);
			spl_point_colors(S);
		}
		else
		{
			init_point_colors();
		}
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		
		if (nPts > 0)
		{
			glVertexPointer(3, GL_FLOAT, 0, pointArray);
			glColorPointer(4, GL_FLOAT, 0, ptColors);
			glDrawElements(GL_POINTS, nPts, GL_UNSIGNED_BYTE, ptIndex);
		}
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
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
	/* Enable the depth buffer */
	glEnable(GL_DEPTH_TEST);

	/* Set clear color */
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClearDepth(1.0);
	
	/*
	glLineWidth(1.5);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	*/
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glLineWidth(0.5);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
}

void timerfunc(int value)
{
	time_t	ltime;
	time(&ltime);
	//cout << "Timer Callback: " << (ltime%10000)-value << endl;
	
	if (select_mode == 1) // Movie
	{
		if (-1 != state_index && !b_pause &&
			state_index < (int)states.v_stateObjects.size() - 1)
		{
			state_index++;
		}
		glutTimerFunc(interval*1000, timerfunc, state_index);
	}
	else if (select_mode == 2) // Batch
	{
		state_index = (int)states.v_stateObjects.size() - 1;
		glutTimerFunc(interval * 1000, timerfunc, state_index);
	}
	else // Step mode
	{
		if (state_index == value && state_index != -1 && !b_pause &&
			(state_index < ((int)states.v_stateObjects.size()) - 1))
		{
			state_index++;
		}
		glutTimerFunc(30*1000, timerfunc, state_index);
	}
		
}
