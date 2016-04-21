#include <iostream>
#include <math.h>
#include <vector>
#include <set>
#include <direct.h>
#include <windows.h>

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glui\glui.h"

#include "gen_codes.h"
#include "Point.h"
#include "Facet.h"
#include "Halfedge.h"
#include "menu.h"

using namespace std;

void clearall()
{
	pts.clearpoints();
	faces.clearall();
	halfedges.clearall();
	pt_excl.clear();
	states.v_stateObjects.clear();
	state_index = -1;
}
/** Function called when the an option of the menu is selected */
void menu(int value)
{
	switch (value)
	{
		case MENU_CLEAR:
		{
			clearall();
			break;
		}
		case MENU_HULL: 
		{
			err_code	status;
			status = draw_3dHull();
			if (E_SUCCESS != status)
			{
				cout << "Hull creation failed\n";
				exit(0);
			}
		}
		case MENU_TAKE_SNAP:
		{
			// Not used
			break;
		}
		case MENU_ROTATE:
		{
			b_rotate = !b_rotate;
			break;
		}
	}
}

bool file_picker(char *filename, int bufsize)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL; 
	ofn.lpstrFile = filename;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = bufsize;
	// If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		//cout << filename << " is selected\n";
		return true;
	}
	else
	{
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) 
		// reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
			case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
			case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
			case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
			case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
			case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
			case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
			case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
			case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
			case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
			case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
			case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
			case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
			case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
			case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
			case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
			default: std::cout << "You cancelled.\n";
		}
		return false;
	}
	
}

void glui_generic_cb(int id)
{
	switch (id)
	{
		case E_MENU:
			break;
		case E_READ_BROWSE: 
		{
			char buffer[1024] = { 0 }; 
			file_picker(buffer, 1024);
			strncpy_s(read_filename, buffer, sizeof(read_filename));
			glui->sync_live();
			//cout << read_filename;
			break;
		}
		case E_WRITE_BROWSE:
		{
			char buffer[1024] = { 0 };
			file_picker(buffer, 1024);
			strncpy_s(write_filename, buffer, sizeof(write_filename));
			glui->sync_live();
			//cout << write_filename;
			break;
		}
		case E_RADIOBUTTON:
		{
			cout << "Button Selected is " << select_mode << endl;
			if (select_mode == 2)
				state_index = states.v_stateObjects.size() -1 ;
			break;
		}
		case E_READFILE_BOX:
		{
			//Manual entry into box 
			break;
		}
		case E_WRITEFILE_BOX:
		{
			//cout << "Write location is " << write_filename << endl;
			break;
		}
		case E_SNAPSHOT_NAME_BOX:
		{
			// Not Used
			break;
		}
		case E_INTRVL_SPINNER:
		{
			//cout << "Value is: " << interval << endl;
			break;
		}
		case E_ZOOM_IN:
		{
			if (zoom < 50)
				zoom += (float)0.3;
			break;
		}
		case E_ZOOM_OUT:
		{
			if (zoom > -50)
				zoom -= (float)0.3;
			break;
		}
		case E_WIRE_FRAME:
		{
			b_wireframe = !b_wireframe;
			break;
		}
		case E_PAUSE:
		{
			b_pause = !b_pause;
			break;
		}
		case E_READ_BUTTON:
		{
			FILE		*fp = NULL;
			err_code	status = E_SUCCESS;

			clearall();

			cout << read_filename << endl;
			fopen_s(&fp, read_filename, "r");
			if (NULL == fp)
			{
				cout << "File Open failed\n" << errno;
				return;
			}
			status = pts.loadpoints(fp);
			if (E_SUCCESS != status)
			{
				cout << "Load points failed\n";
				exit(0);
			}

			fclose(fp);
			break;
		}
		case E_CLEAR_BUTTON:
		{
			clearall();
			break;
		}
		case E_RFACE_CLR_LISTBOX:
		{
			int i = rface_pt_clrlist->get_int_val();
			remv_face_color[0] = color_values[i][0];
			remv_face_color[1] = color_values[i][1];
			remv_face_color[2] = color_values[i][2];
			remv_face_color[4] = 0.7;

			break;
		}
		case E_FACE_CLR_LISTBOX:
		{
			int i = face_pt_clrlist->get_int_val();
			normal_face_color[0] = color_values[i][0];
			normal_face_color[1] = color_values[i][1];
			normal_face_color[2] = color_values[i][2];
			normal_face_color[4] = 0.7;

			break;
		}
		case E_HLPT_CLR_LISTBOX:
		{
			int i = hlt_pt_clrlist->get_int_val();
			spl_pt_color[0] = color_values[i][0];
			spl_pt_color[1] = color_values[i][1];
			spl_pt_color[2] = color_values[i][2];
			spl_pt_color[4] = 0.7;

			break;
		}case E_INTPT_CLR_LISTBOX:
		{
			int i = int_pt_clrlist->get_int_val();
			int_pt_color[0] = color_values[i][0];
			int_pt_color[1] = color_values[i][1];
			int_pt_color[2] = color_values[i][2];
			int_pt_color[4] = 0.7;

			break;
		}case E_EXTPT_CLR_LISTBOX:
		{
			int i = ext_pt_clrlist->get_int_val();
			ext_pt_color[0] = color_values[i][0];
			ext_pt_color[1] = color_values[i][1];
			ext_pt_color[2] = color_values[i][2];
			ext_pt_color[4] = 0.7;

			break;
		}
		case E_LINEPT_CLR_LISTBOX:
		{
			int i = line_pt_clrlist->get_int_val();
			norm_line_color[0] = color_values[i][0];
			norm_line_color[1] = color_values[i][1];
			norm_line_color[2] = color_values[i][2];
			norm_line_color[4] = 0.7;
			break;
		}
		case E_GEN_RAND_PTS:
		{
			clearall();
			int objType = objectList->get_int_val();
			switch (objType)
			{
			case 0:
				hollow_sphere[0][2]; break;
			case 1: 
				solid_sphere[0][2]; break;
			case 2:
				hollow_cube[0][2]; break;
			case 3:
				solid_cube[0][2]; break;
			case 4:
				hollow_tetrahedron[0][2]; break;
			case 5:
				solid_tetrahedron[0][2]; break;
			default:
				break;
			}

			break;
		}
		break;
		default:
		{
			break;
		}
	}
}

/** Generates the menu */
void makeMenu()
{
	/* Generate the menu */
	menuID = glutCreateMenu(menu);
	glutAddMenuEntry("3D Hull", MENU_HULL);
	glutAddMenuEntry("Rotate", MENU_ROTATE);
	//glutAddMenuEntry("Take Snap", MENU_TAKE_SNAP);
	glutAddMenuEntry("Clear", MENU_CLEAR);

	/* Attach menu to the right click */
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}
