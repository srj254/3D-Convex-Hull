#pragma once
#include <vector>
#include <set>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glui\glui.h"

#define	 DEBUG	1

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
	E_CLEAR_BUTTON,
	E_READ_BROWSE,
	E_WRITE_BROWSE,
	E_RAND_POINTS,
	E_READ_BUTTON
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
extern int		rand_points;
extern char		read_filename[512];
extern char		write_filename[512];
extern char		snapshot_name_prefix[512];

extern float			angle;
extern float			zoom;
extern bool				b_rotate;
extern bool				b_wireframe;
extern HANDLE			ghSemaphore;

extern float			pointArray[];
extern GLubyte			ptIndex[];
extern int				nPts;
extern GLubyte			hullFaces[];
extern int				nFaces;
extern GLubyte			hullEdges[];
extern int				nHEdges;

extern GLUI				*glui;

typedef enum ERROR_CODE_T {
	E_SUCCESS,
	E_FAILURE
}err_code;

void getPtsInArray();
void getFacesInArray();



