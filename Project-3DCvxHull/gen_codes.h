#pragma once
#include <vector>
#include <set>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glui\glui.h"

#define	 DEBUG	0

typedef enum menu_IDList_T
{
	E_MENU,		
	E_RADIOBUTTON,
	E_READFILE_BOX,
	E_WRITEFILE_BOX,
	E_SNAPSHOT_NAME_BOX,
	E_INTRVL_SPINNER,
	E_ZOOM_IN,
	E_ZOOM_OUT,
	E_WIRE_FRAME,
	E_PAUSE,
	E_CLEAR_BUTTON,
	E_READ_BROWSE,
	E_WRITE_BROWSE,
	E_READ_BUTTON,
	E_OBJECT_LISTBOX,
	E_HLPT_CLR_LISTBOX,
	E_FACE_CLR_LISTBOX,
	E_RFACE_CLR_LISTBOX,
	E_INTPT_CLR_LISTBOX,
	E_EXTPT_CLR_LISTBOX,
	E_LINEPT_CLR_LISTBOX,
	E_GEN_RAND_PTS
}IDlist;

typedef enum CONFLICT_CODES_T
{
	E_NEW,
	E_NO_CNFLCT,
	E_YES_CNFLCT,
	E_END
}conflict_code;

typedef enum PT_ORIENT_T {
	E_BELOW,
	E_EQUAL,
	E_ABOVE,
}pt_orient;

#define		PRINT_LINE		//printf("%s@%d\n", __FILE__, __LINE__)

/* Menu IDs */
extern int		menuID;
extern int		pointSubmenuID;
extern int		fileSubmenuID;
extern int		radiobutton_menuID;
extern int		readfile_boxID;
extern int		writefile_boxID;
extern int		interval_spinnerID;

/* GLUT window parameters */
extern int		windowWidth;
extern int		windowHeight;
extern int		windowID;

/* GLUI related live variables */
extern int		interval;
extern int		select_mode;
extern int		select_object;
extern char		read_filename[512];
extern char		write_filename[512];

/* Globals to be modified by menu functions */
extern bool		b_wireframe;
extern bool		b_rotate;
extern bool		b_pause;
extern float	angle;
extern float	v_x, v_y, v_z;
extern float	zoom;

/*Globals for OpenGL Drawing */
extern int			nPts;
extern GLubyte		ptIndex[];
extern float		pointArray[];
extern std::vector<int>	hull_ptIndex;
extern float		ptColors[];
extern unsigned int	tri_vertices[];
extern unsigned int	edg_vertices[];
extern int			state_index;
extern char			color_names[][32];
extern float		color_values[][3];
extern int			object_values[6];
extern char			object_names[][64];
extern float		normal_face_color[];
extern float		new_face_color[];
extern float		remv_face_color[];
extern float		norm_line_color[];
extern float		horizon_line_color[];
extern float		init_pt_color[];
extern float		spl_pt_color[];
extern float		ext_pt_color[];
extern float		int_pt_color[];
extern float		hull_pt_color[];

/* Global GLUI handle */
extern GLUI			*glui;
extern GLUI_Listbox *hlt_pt_clrlist;
extern GLUI_Listbox *int_pt_clrlist;
extern GLUI_Listbox *ext_pt_clrlist;
extern GLUI_Listbox *face_pt_clrlist;
extern GLUI_Listbox *rface_pt_clrlist;
extern GLUI_Listbox *line_pt_clrlist;
extern GLUI_Checkbox *X;
extern GLUI_Checkbox *Y;
extern GLUI_Checkbox *Z;

/* Mouse and keyboard globals */
extern int		mouse_curX;
extern int		mouse_curY;

/* Regular objects, random points */
extern float hollow_sphere[][3];
extern float hollow_tetrahedron[][3];
extern float hollow_cube[][3];
extern float solid_sphere[][3];
extern float solid_tetrahedron[][3];
extern float solid_cube[][3];

/* Generic Error code definition */
typedef enum ERROR_CODE_T {
	E_SUCCESS,
	E_FAILURE
}err_code;

/* Generic/Opengl render functions used */
void getPtsInArray();
void changeSize(int w, int h);
void display();
void idle();
void init();
void timerfunc(int value);

/*Objects*/
void get_solid_tethdrn();
void get_solid_cube();
void get_solid_sphere();

void get_hollow_tethdrn();
void get_hollow_cube();
void get_hollow_sphere();




