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
#include "StateObject.h"
#include "menu.h"

using namespace std;

/** Window information */
int	windowWidth = 600;
int	windowHeight = windowWidth;
int windowXpos	= 100;
int windowYpos = 100;
int	windowID = -1;

/* GLUI related live variables */
char	read_filename[512] = { 0 };
char	write_filename[512] = { 0 };
char	snapshot_name_prefix[512] = { 0 };
int		interval = 1;
int		rand_points = 1;
int		select_mode = 0;

/** Menu IDs */
int		menuID = E_MENU;
int		fileSubmenuID = E_READ_BROWSE;
int		radiobutton_menuID = E_RADIOBUTTON;
int		readfile_boxID = E_READFILE_BOX;
int		writefile_boxID = E_WRITEFILE_BOX;
int		interval_spinnerID = E_INTRVL_SPINNER;

/* Points, Faces, Halfedges */
Points			pts;			// Store all points 
allFaces		faces;			// Store all curent faces
allHalfedges	halfedges;		// Store all possible Halfedges
ConflictGraph	cnflct_graph;	// Conflict Graph of Faces Vs Pts
vector<Pt>		pt_excl;
allstates		states; 

float			angle	= 0.00;
float			zoom	= 0.00;

float			pointArray[MAX_POINTS * 3];
GLubyte			ptIndex[MAX_POINTS];
int				nPts = 0;
GLubyte			hullFaces[3 * MAX_FACES];
int				nFaces = 0;
GLubyte			hullEdges[2 * MAX_HALFEDGES];
int				nHEdges = 0;

bool			b_rotate = false;
bool			b_wireframe = false;

GLUI			*glui = NULL;

void changeSize(int w, int h);
void display();
void idle();
void init();

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
	GLUI_Button *next_button = glui->add_button_to_panel(app_modes,
						"Next Step", -1, glui_generic_cb);
	GLUI_Spinner *step_points = glui->add_spinner_to_panel(app_modes,
							"Number of random points", GLUI_SPINNER_INT, 
							&rand_points);
	step_points->set_speed(0.001);
	step_points->set_w(5);
	step_points->set_int_limits(1, 120, GLUI_LIMIT_WRAP);
	glui->add_button_to_panel(app_modes, "Generate", 
								E_RAND_POINTS, glui_generic_cb);

	glui->add_column_to_panel(parent_panel, false);
	GLUI_Panel *objTransfrm = glui->add_panel_to_panel(parent_panel, 
													"Transformation");
	objTransfrm->set_alignment(GLUI_ALIGN_LEFT);
	
	GLUI_Checkbox *wireframe_box = glui->add_checkbox_to_panel(objTransfrm,
								"Wireframe", nullptr, E_WIRE_FRAME,
								glui_generic_cb);
	GLUI_Panel *subpanel = glui->add_panel_to_panel(objTransfrm, "Zoom",
												GLUI_PANEL_EMBOSSED);
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
	GLUI_EditText *write_file = glui->add_edittext_to_panel(fileOpsPanel, 
					"Write Location", GLUI_EDITTEXT_TEXT, write_filename, 
					E_WRITEFILE_BOX, glui_generic_cb);
	write_file->set_w(200);
	GLUI_EditText *snapshot_name = glui->add_edittext_to_panel(fileOpsPanel, 
					"Image Name Prefix", GLUI_EDITTEXT_TEXT, snapshot_name_prefix,
					E_SNAPSHOT_NAME_BOX, glui_generic_cb);
	snapshot_name->set_w(200);
	glui->add_column_to_panel(fileOpsPanel, false);

	(glui->add_button_to_panel(fileOpsPanel, "Browse", E_READ_BROWSE, 
					glui_generic_cb))->set_w(30);

	glui->add_button_to_panel(fileOpsPanel, "Browse", E_WRITE_BROWSE, 
					glui_generic_cb)->set_w(30);
	glui->add_column_to_panel(fileOpsPanel, false);
	glui->add_button_to_panel(fileOpsPanel, "Read", E_READ_BUTTON,
					glui_generic_cb)->set_w(30);

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
