#pragma once
#include "StateObject.h"

typedef enum MENU_OPTIONS_T {
	MENU_CLEAR,
	MENU_HULL,
	MENU_ROTATE,
	MENU_TAKE_SNAP
}menu_options;

/* API definition */
void makeMenu();
void keyboard(unsigned char k, int x, int y);
void mouse(int button, int state, int x, int y);
void activeMotion(int x, int y);
void passiveMotion(int x, int y);
void glui_generic_cb(int id);
bool file_picker(char *filename, int bufsize);

/* Menu handlers */
err_code draw_3dHull();
bool check_collinear(Pt &a, Pt &b, Pt &c);
PT_ORIENT_T check_coplanar(Pt &p1, Pt &p2, Pt &p3, Pt &check);
bool init_pointswaps();
err_code crt_2face_triangle(Pt &a, Pt &b, Pt &c);
void init_norm_face_color();
void init_new_face_color();
void init_remv_face_color();
void init_norm_line_color();
void init_horizon_line_color();
void init_point_colors();
void spl_point_colors(StateObject &S);

