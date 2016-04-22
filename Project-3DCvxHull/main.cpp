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

/** Window information */
int	windowWidth = 900;
int	windowHeight = windowWidth;
int windowXpos	= 300;
int windowYpos = 0;
int	windowID = -1;

/** Menu IDs */
int		menuID = E_MENU;
int		fileSubmenuID = E_READ_BROWSE;
int		radiobutton_menuID = E_RADIOBUTTON;
int		readfile_boxID = E_READFILE_BOX;
int		writefile_boxID = E_WRITEFILE_BOX;
int		interval_spinnerID = E_INTRVL_SPINNER;

/* Points, Faces, Halfedges, States, Conflict Graph */
Points			pts;			// Store all points 
allFaces		faces;			// Store all curent faces
allHalfedges	halfedges;		// Store all possible Halfedges
ConflictGraph	cnflct_graph;	// Conflict Graph of Faces Vs Pts
vector<Pt>		pt_excl;		// Store all points considered
allstates		states;			// Store every intermediate states 

/* Globals used by menu functions */
bool			b_rotate = false;
bool			b_wireframe = false;
bool			b_pause = false;
float			angle	= 90;
float			v_x		= 0.0;
float			v_y		= 0.0;
float			v_z		= 0.0;
float			zoom	= 0.00;

/* Globals for OpenGL drawing */
GLubyte			ptIndex[MAX_POINTS];
std::vector<int>	hull_ptIndex;
int				nPts = 0;
float			pointArray[MAX_POINTS * 3];
float			ptColors[MAX_POINTS * 4];
unsigned int	tri_vertices[3] = { 0 };
unsigned int	edg_vertices[2] = { 0 };
int				state_index = -1;

/* GLUI related live variables */
char	read_filename[512] = { 0 };
char	write_filename[512] = { 0 };
int		interval = 1;
int		select_mode = 0;
int		select_object = 0;

/* Global GLUI Handle */
GLUI	*glui = NULL;
GLUI_Listbox *hlt_pt_clrlist = NULL;
GLUI_Listbox *int_pt_clrlist = NULL;
GLUI_Listbox *ext_pt_clrlist = NULL;
GLUI_Listbox *face_pt_clrlist = NULL;
GLUI_Listbox *rface_pt_clrlist = NULL;
GLUI_Listbox *line_pt_clrlist = NULL;
GLUI_Checkbox *X = NULL;
GLUI_Checkbox *Y = NULL;
GLUI_Checkbox *Z = NULL;

/* Mouse and keyboard globals */
int		mouse_curX = 0;
int		mouse_curY = 0;

/*Dummy */
int dummy = 0;

/** Main function */
int main(int argc, char **argv)
{
	/* Initialize the GLUT window */
	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(windowXpos+100, windowYpos);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	windowID = glutCreateWindow("CS531-3D-Convex-Hull");

	/* Set OpenGL initial state */
	init();

	/* Callback functions */
	glutDisplayFunc(display);
	// glutReshapeFunc(changeSize);
	/* Registered with GLUI, hence removed registration with GLUT */
	glutIdleFunc(idle); 
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(activeMotion);
	glutPassiveMotionFunc(passiveMotion);
	time_t	ltime; time(&ltime);
	glutTimerFunc(2*1000, timerfunc, ltime%10000);

	/* Generate the menu */
	makeMenu();

	/****************************************/
	/*         GLUI settings	           */
	/****************************************/

	glui = GLUI_Master.create_glui("GLUI", 0, 0, 0);
	
	GLUI_Panel *parent_panel = glui->add_panel("All Menu Here", GLUI_PANEL_NONE);

	GLUI_Panel *app_modes = glui->add_panel_to_panel(parent_panel, 
													"Application modes");
	app_modes->set_alignment(GLUI_ALIGN_LEFT);
	GLUI_RadioGroup *modes = glui->add_radiogroup_to_panel(
					app_modes, &select_mode,
					radiobutton_menuID, glui_generic_cb);
	glui->add_radiobutton_to_group(modes, "Step");
	glui->add_radiobutton_to_group(modes, "Movie");
	glui->add_radiobutton_to_group(modes, "Batch");
	GLUI_Spinner *step_interval = glui->add_spinner_to_panel(app_modes, 
							"Interval(sec)", GLUI_SPINNER_INT, &interval,
							E_INTRVL_SPINNER, glui_generic_cb);
	step_interval->set_speed(0.001);
	step_interval->set_w(5);
	step_interval->set_int_limits(1, 120, GLUI_LIMIT_WRAP);
	GLUI_Checkbox *pause_box = glui->add_checkbox_to_panel(app_modes,
		"Pause", nullptr, E_PAUSE, glui_generic_cb);

	glui->add_column_to_panel(parent_panel, false);
	GLUI_Panel *objTransfrm = glui->add_panel_to_panel(parent_panel, 
													"Transformation");
	objTransfrm->set_alignment(GLUI_ALIGN_LEFT);
	
	GLUI_Checkbox *wireframe_box = glui->add_checkbox_to_panel(objTransfrm,
								"Wireframe", nullptr, E_WIRE_FRAME,
								glui_generic_cb);
	GLUI_Panel	*rotate_subPanel = glui->add_panel_to_panel(objTransfrm, "Rotate");
	X = glui->add_checkbox_to_panel(rotate_subPanel, "X");
	X->set_w(20);
	glui->add_column_to_panel(rotate_subPanel, false);
	Y = glui->add_checkbox_to_panel(rotate_subPanel, "Y");
	Y->set_w(20);
	glui->add_column_to_panel(rotate_subPanel, false);
	Z = glui->add_checkbox_to_panel(rotate_subPanel, "Z");
	Z->set_w(20);
	GLUI_Panel *subpanel = glui->add_panel_to_panel(objTransfrm, "Zoom");
	GLUI_Button* button;
	button = glui->add_button_to_panel(subpanel, "+", E_ZOOM_IN, glui_generic_cb);
	button->set_w(20);
	glui->add_column_to_panel(subpanel, false);
	button = glui->add_button_to_panel(subpanel, "-", E_ZOOM_OUT, glui_generic_cb);
	button->set_w(20);

	GLUI_Panel *fileOpsPanel = glui->add_panel("File Operations");
	GLUI_EditText *read_file = glui->add_edittext_to_panel(fileOpsPanel, 
					"Read Location", GLUI_EDITTEXT_TEXT, read_filename,
					E_READFILE_BOX, glui_generic_cb);
	read_file->set_w(200);
	//GLUI_EditText *write_file = glui->add_edittext_to_panel(fileOpsPanel, 
	//				"Write Location", GLUI_EDITTEXT_TEXT, write_filename, 
	//				E_WRITEFILE_BOX, glui_generic_cb);
	//write_file->set_w(200);
	glui->add_column_to_panel(fileOpsPanel, false);

	(glui->add_button_to_panel(fileOpsPanel, "Browse", E_READ_BROWSE, 
					glui_generic_cb))->set_w(30);

	//glui->add_button_to_panel(fileOpsPanel, "Browse", E_WRITE_BROWSE, 
	//				glui_generic_cb)->set_w(30);
	glui->add_column_to_panel(fileOpsPanel, false);
	glui->add_button_to_panel(fileOpsPanel, "Read", E_READ_BUTTON,
					glui_generic_cb)->set_w(30);

	if (false)
	{
		GLUI_Panel *clr_panel = glui->add_panel("Choose colors");
		hlt_pt_clrlist = glui->add_listbox_to_panel(clr_panel,
			"Random Point  ", NULL, E_HLPT_CLR_LISTBOX, glui_generic_cb);
		hlt_pt_clrlist->set_w(100);
		for (unsigned ii = 0; ii < 147; ii++)
			hlt_pt_clrlist->add_item(ii, color_names[ii]);

		int_pt_clrlist = glui->add_listbox_to_panel(clr_panel,
			"Interior Point    ", NULL, E_INTPT_CLR_LISTBOX, glui_generic_cb);
		int_pt_clrlist->set_w(100);
		for (unsigned ii = 0; ii < 147; ii++)
			int_pt_clrlist->add_item(ii, color_names[ii]);

		ext_pt_clrlist = glui->add_listbox_to_panel(clr_panel,
			"Exterior Point   ", NULL, E_EXTPT_CLR_LISTBOX, glui_generic_cb);
		ext_pt_clrlist->set_w(100);
		for (unsigned ii = 0; ii < 147; ii++)
			ext_pt_clrlist->add_item(ii, color_names[ii]);

		//glui->add_column_to_panel(clr_panel, false);
		face_pt_clrlist = glui->add_listbox_to_panel(clr_panel,
			"Faces on Hull  ", NULL, E_FACE_CLR_LISTBOX, glui_generic_cb);
		face_pt_clrlist->set_w(100);
		for (unsigned ii = 0; ii < 147; ii++)
			face_pt_clrlist->add_item(ii, color_names[ii]);

		rface_pt_clrlist = glui->add_listbox_to_panel(clr_panel,
			"Faces to Delete", NULL, E_RFACE_CLR_LISTBOX, glui_generic_cb);
		rface_pt_clrlist->set_w(100);
		for (unsigned ii = 0; ii < 147; ii++)
			rface_pt_clrlist->add_item(ii, color_names[ii]);

		line_pt_clrlist = glui->add_listbox_to_panel(clr_panel,
			"Edges on Hull  ", NULL, E_LINEPT_CLR_LISTBOX, glui_generic_cb);
		line_pt_clrlist->set_w(100);
		for (unsigned ii = 0; ii < 147; ii++)
			line_pt_clrlist->add_item(ii, color_names[ii]);

	}

	{

		GLUI_Panel *obj_panel = glui->add_panel("3D Objects");
		GLUI_RadioGroup *objlist = glui->add_radiogroup_to_panel(obj_panel, 
							&select_object, E_OBJECT_LISTBOX, glui_generic_cb);
		for (unsigned ii = 0; ii < 6; ii++)
			glui->add_radiobutton_to_group(objlist, object_names[ii]);
		
		glui->add_button_to_panel(obj_panel, "Generate points", E_GEN_RAND_PTS,
			glui_generic_cb);
	}

	GLUI_Panel *exit_panel = glui->add_panel("Exit/Clear Panel", GLUI_PANEL_NONE);
	glui->add_button_to_panel(exit_panel, "Exit Application", 0, 
								(GLUI_Update_CB)exit);
	glui->add_column_to_panel(exit_panel, false);
	glui->add_button_to_panel(exit_panel, "Clear all", E_CLEAR_BUTTON, 
											glui_generic_cb);


	glui->set_main_gfx_window(windowID);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(idle);

	/****************************************/
	/*         Here ends the GLUI code      */
	/****************************************/

	/* Start the main GLUT loop */
	glutMainLoop();
}
