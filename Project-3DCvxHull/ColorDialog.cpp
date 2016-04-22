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

COLORREF hlt_point = RGB(255, 255, 255);
COLORREF evaluated_point = RGB(255, 255, 255);
COLORREF exterior_point = RGB(255, 255, 255);
COLORREF interior_point = RGB(255, 255, 255);

COLORREF normal_face = RGB(255, 255, 255);
COLORREF deleted_face = RGB(255, 255, 255);
COLORREF added_face = RGB(255, 255, 255);

// adapted from: 
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646829(v=vs.85).aspx#choosing_color
// returns true if user clicked OK
// color - the initial color to present the user in the 
// dialog AND will be updated if the user clicks OK
bool doPickColorDialog(COLORREF* color) {
	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	HWND hwnd = nullptr;            // owner window
	static DWORD rgbCurrent;        // initial color selection

	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = *color;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	bool result = ChooseColor(&cc) == TRUE; //VC++ is dumb, so fix a warning
	*color = cc.rgbResult;
	return result;
}