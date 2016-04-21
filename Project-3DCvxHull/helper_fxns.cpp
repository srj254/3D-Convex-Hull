#include <iostream>
#include <math.h>
#include <set>
#include "gen_codes.h"
#include "Point.h"
#include "Facet.h"
#include "Halfedge.h"
#include "ConflictGraph.h"
#include "StateObject.h"
#include "menu.h"

using namespace std;

bool check_collinear(Pt &a, Pt &b, Pt &c)
{
	double ratio1 = 0, ratio2 = 0, ratio3 = 0;
	
	ratio1 = (b.X() - a.X()) / (c.X() - a.X());
	ratio2 = (b.Y() - a.Y()) / (c.Y() - a.Y());
	ratio3 = (b.Z() - a.Z()) / (c.Z() - a.Z());

	if ((ratio1 == ratio2) && (ratio3 == ratio2))
		return true;

	return false;
}

err_code crt_2face_triangle(Pt &a, Pt &b, Pt &c)
{
	vector<int>	facet1_edges;
	vector<int>	facet2_edges;
	int			index;

	index = halfedges.find_halfedge(a, b);
	facet1_edges.push_back(index);
	halfedges.v_halfedges[index].print_halfedge(); cout << endl;
	index = halfedges.find_halfedge(b, c);
	facet1_edges.push_back(index);
	halfedges.v_halfedges[index].print_halfedge(); cout << endl;
	index = halfedges.find_halfedge(c, a);
	facet1_edges.push_back(index);
	halfedges.v_halfedges[index].print_halfedge(); cout << endl;

	index = halfedges.find_halfedge(b, a);
	facet2_edges.push_back(index);
	halfedges.v_halfedges[index].print_halfedge(); cout << endl;
	index = halfedges.find_halfedge(a, c);
	facet2_edges.push_back(index);
	halfedges.v_halfedges[index].print_halfedge(); cout << endl;
	index = halfedges.find_halfedge(c, b);
	facet2_edges.push_back(index);
	halfedges.v_halfedges[index].print_halfedge(); cout << endl;

	try
	{
		Facet	F1(facet1_edges, pts.v_pts.size(), -1),
				F2(facet2_edges, pts.v_pts.size(), -1);

		faces.addFace(F1);
		faces.addFace(F2);
	}
	catch (err_code &e)
	{
		cout << "Failure: Creating initial facets: "<< e << endl;
		exit(0);
	}

	return E_SUCCESS;
}

int fchange = 0;
int pchange = 0;

void getPtsInArray()
{
	float	px, py, pz; 
	int		j = 0;

	nPts = 0;
	for (unsigned i = 0; i < pts.v_pts.size(); i++)
	{
		px = pts.v_pts.at(i).X();
		py = pts.v_pts.at(i).Y();
		pz = pts.v_pts.at(i).Z();
		pointArray[j] = px;  j++;
		pointArray[j] = py;  j++;
		pointArray[j] = pz; j++;
		ptIndex[i] = i;
		nPts++;
	}

#if DEBUG
	{
		cout << "Points: " << nPts << endl;
		for (unsigned c = 0; c < pts.v_pts.size(); c++)
			pts.v_pts.at(c).print_point();
		pchange = 0;
	}
#endif
	return;
}

float normal_face_color[4] = {0.6, 0.6, 0.6, 0.7};
float remv_face_color[4] = { 0.3, 0.3, 0.3, 0.7 };
float norm_line_color[4] = { 0.0, 0.0, 0.0, 0.3 }; 
float horizon_line_color[4] = { 1.0, 1.0, 0.0, 1.0 };
float init_pt_color[4] = { 0.0, 0.0, 0.0, 0.7 };
float spl_pt_color[4] = { 0, 0, 1.0, 0.7 };
float ext_pt_color[4] = { 1, 0, 0.0, 0.7 };
float int_pt_color[4] = { 0, 1, 0.0, 0.7 };

void init_norm_face_color()
{
	for (unsigned i = 0; i < pts.v_pts.size(); i++)
	{
		ptColors[i * 4 + 0] = normal_face_color[0];
		ptColors[i * 4 + 1] = normal_face_color[1];
		ptColors[i * 4 + 2] = normal_face_color[2];
		ptColors[i * 4 + 3] = normal_face_color[3];
	}
}


void init_remv_face_color()
{
	for (unsigned i = 0; i < pts.v_pts.size(); i++)
	{
		ptColors[i * 4 + 0] = remv_face_color[0];
		ptColors[i * 4 + 1] = remv_face_color[1];
		ptColors[i * 4 + 2] = remv_face_color[2];
		ptColors[i * 4 + 3] = remv_face_color[3];
	}
}


void init_norm_line_color()
{
	for (unsigned i = 0; i < pts.v_pts.size(); i++)
	{
		ptColors[i * 4 + 0] = norm_line_color[0];
		ptColors[i * 4 + 1] = norm_line_color[1];
		ptColors[i * 4 + 2] = norm_line_color[2];
		ptColors[i * 4 + 3] = norm_line_color[3];
	}
}

void init_horizon_line_color()
{
	for (unsigned i = 0; i < pts.v_pts.size(); i++)
	{
		ptColors[i * 4 + 0] = horizon_line_color[0];
		ptColors[i * 4 + 1] = horizon_line_color[1];
		ptColors[i * 4 + 2] = horizon_line_color[2];
		ptColors[i * 4 + 3] = horizon_line_color[3];
	}
}


void spl_point_colors(StateObject &S)
{
	for (unsigned i = 0; i < pts.v_pts.size(); i++)
	{
		if (i == S.highlight_pt)
		{
			ptColors[i * 4 + 0] = spl_pt_color[0];
			ptColors[i * 4 + 1] = spl_pt_color[1];
			ptColors[i * 4 + 2] = spl_pt_color[2]; // Blue
			ptColors[i * 4 + 3] = spl_pt_color[3];
		}
		else if (std::find( S.exterior_pts.begin(), 
							S.exterior_pts.end(), i) != 
							S.exterior_pts.end())
		{
			ptColors[i * 4 + 0] = ext_pt_color[0]; // Red
			ptColors[i * 4 + 1] = ext_pt_color[1];
			ptColors[i * 4 + 2] = ext_pt_color[2];
			ptColors[i * 4 + 3] = ext_pt_color[3];
		}
		else
		{
			ptColors[i * 4 + 0] = int_pt_color[0];
			ptColors[i * 4 + 1] = int_pt_color[1]; // Red
			ptColors[i * 4 + 2] = int_pt_color[2];
			ptColors[i * 4 + 3] = int_pt_color[3];
		}
	}
}

void init_point_colors()
{
	for (unsigned i = 0; i < pts.v_pts.size(); i++)
	{
		ptColors[i * 4 + 0] = init_pt_color[0];
		ptColors[i * 4 + 1] = init_pt_color[1];
		ptColors[i * 4 + 2] = init_pt_color[2];
		ptColors[i * 4 + 3] = init_pt_color[3];
	}
}

char color_names[][32] = 
{ "aliceblue","antiquewhite","aqua","aquamarine","azure",
  "beige","bisque","black","blanchedalmond","blue","blueviolet",
  "brown","burlywood","cadetblue","chartreuse","chocolate","coral",
  "cornflowerblue","cornsilk","crimson","cyan","darkblue","darkcyan",
  "darkgoldenrod","darkgray","darkgreen","darkgrey","darkkhaki",
  "darkmagenta","darkolivegreen","darkorange","darkorchid","darkred",
	"darksalmon","darkseagreen","darkslateblue","darkslategray",
	"darkslategrey","darkturquoise","darkviolet","deeppink","deepskyblue",
	"dimgray","dimgrey","dodgerblue","firebrick","floralwhite","forestgreen",
	"fuchsia","gainsboro","ghostwhite","gold","goldenrod","gray","green",
	"greenyellow","grey","honeydew","hotpink","indianred","indigo","ivory",
	"khaki","lavender","lavenderblush","lawngreen","lemonchiffon","lightblue",
	"lightcoral","lightcyan","lightgoldenrodyellow","lightgray","lightgreen",
	"lightgrey","lightpink","lightsalmon","lightseagreen","lightskyblue",
	"lightslategray","lightslategrey","lightsteelblue","lightyellow","lime",
	"limegreen","linen","magenta","maroon","mediumaquamarine","mediumblue",
	"mediumorchid","mediumpurple","mediumseagreen","mediumslateblue",
	"mediumspringgreen","mediumturquoise","mediumvioletred","midnightblue",
	"mintcream","mistyrose","moccasin","navajowhite","navy","oldlace","olive",
	"olivedrab","orange","orangered","orchid","palegoldenrod","palegreen",
	"paleturquoise","palevioletred","papayawhip","peachpuff","peru","pink",
	"plum","powderblue","purple","red","rosybrown","royalblue","saddlebrown",
	"salmon","sandybrown","seagreen","seashell","sienna","silver","skyblue",
	"slateblue","slategray","slategrey","snow","springgreen","steelblue","tan",
	"teal","thistle","tomato","turquoise","violet","wheat","white","whitesmoke",
	"yellow","yellowgreen"
};

float color_values[][3] = { 
{ 0.941f, 0.973f, 1.000f },
{ 0.980f, 0.922f, 0.843f },
{ 0.000f, 1.000f, 1.000f },
{ 0.498f, 1.000f, 0.831f },
{ 0.941f, 1.000f, 1.000f },
{ 0.961f, 0.961f, 0.863f },
{ 1.000f, 0.894f, 0.769f },
{ 0.000f, 0.000f, 0.000f },
{ 1.000f, 0.922f, 0.804f },
{ 0.000f, 0.000f, 1.000f },
{ 0.541f, 0.169f, 0.886f },
{ 0.647f, 0.165f, 0.165f },
{ 0.871f, 0.722f, 0.529f },
{ 0.373f, 0.620f, 0.627f },
{ 0.498f, 1.000f, 0.000f },
{ 0.824f, 0.412f, 0.118f },
{ 1.000f, 0.498f, 0.314f },
{ 0.392f, 0.584f, 0.929f },
{ 1.000f, 0.973f, 0.863f },
{ 0.863f, 0.078f, 0.235f },
{ 0.000f, 1.000f, 1.000f },
{ 0.000f, 0.000f, 0.545f },
{ 0.000f, 0.545f, 0.545f },
{ 0.722f, 0.525f, 0.043f },
{ 0.663f, 0.663f, 0.663f },
{ 0.000f, 0.392f, 0.000f },
{ 0.663f, 0.663f, 0.663f },
{ 0.741f, 0.718f, 0.420f },
{ 0.545f, 0.000f, 0.545f },
{ 0.333f, 0.420f, 0.184f },
{ 1.000f, 0.549f, 0.000f },
{ 0.600f, 0.196f, 0.800f },
{ 0.545f, 0.000f, 0.000f },
{ 0.914f, 0.588f, 0.478f },
{ 0.561f, 0.737f, 0.561f },
{ 0.282f, 0.239f, 0.545f },
{ 0.184f, 0.310f, 0.310f },
{ 0.184f, 0.310f, 0.310f },
{ 0.000f, 0.808f, 0.820f },
{ 0.580f, 0.000f, 0.827f },
{ 1.000f, 0.078f, 0.576f },
{ 0.000f, 0.749f, 1.000f },
{ 0.412f, 0.412f, 0.412f },
{ 0.412f, 0.412f, 0.412f },
{ 0.118f, 0.565f, 1.000f },
{ 0.698f, 0.133f, 0.133f },
{ 1.000f, 0.980f, 0.941f },
{ 0.133f, 0.545f, 0.133f },
{ 1.000f, 0.000f, 1.000f },
{ 0.863f, 0.863f, 0.863f },
{ 0.973f, 0.973f, 1.000f },
{ 1.000f, 0.843f, 0.000f },
{ 0.855f, 0.647f, 0.125f },
{ 0.502f, 0.502f, 0.502f },
{ 0.000f, 0.502f, 0.000f },
{ 0.678f, 1.000f, 0.184f },
{ 0.502f, 0.502f, 0.502f },
{ 0.941f, 1.000f, 0.941f },
{ 1.000f, 0.412f, 0.706f },
{ 0.804f, 0.361f, 0.361f },
{ 0.294f, 0.000f, 0.510f },
{ 1.000f, 1.000f, 0.941f },
{ 0.941f, 0.902f, 0.549f },
{ 0.902f, 0.902f, 0.980f },
{ 1.000f, 0.941f, 0.961f },
{ 0.486f, 0.988f, 0.000f },
{ 1.000f, 0.980f, 0.804f },
{ 0.678f, 0.847f, 0.902f },
{ 0.941f, 0.502f, 0.502f },
{ 0.878f, 1.000f, 1.000f },
{ 0.980f, 0.980f, 0.824f },
{ 0.827f, 0.827f, 0.827f },
{ 0.565f, 0.933f, 0.565f },
{ 0.827f, 0.827f, 0.827f },
{ 1.000f, 0.714f, 0.757f },
{ 1.000f, 0.627f, 0.478f },
{ 0.125f, 0.698f, 0.667f },
{ 0.529f, 0.808f, 0.980f },
{ 0.467f, 0.533f, 0.600f },
{ 0.467f, 0.533f, 0.600f },
{ 0.690f, 0.769f, 0.871f },
{ 1.000f, 1.000f, 0.878f },
{ 0.000f, 1.000f, 0.000f },
{ 0.196f, 0.804f, 0.196f },
{ 0.980f, 0.941f, 0.902f },
{ 1.000f, 0.000f, 1.000f },
{ 0.502f, 0.000f, 0.000f },
{ 0.400f, 0.804f, 0.667f },
{ 0.000f, 0.000f, 0.804f },
{ 0.729f, 0.333f, 0.827f },
{ 0.576f, 0.439f, 0.859f },
{ 0.235f, 0.702f, 0.443f },
{ 0.482f, 0.408f, 0.933f },
{ 0.000f, 0.980f, 0.604f },
{ 0.282f, 0.820f, 0.800f },
{ 0.780f, 0.082f, 0.522f },
{ 0.098f, 0.098f, 0.439f },
{ 0.961f, 1.000f, 0.980f },
{ 1.000f, 0.894f, 0.882f },
{ 1.000f, 0.894f, 0.710f },
{ 1.000f, 0.871f, 0.678f },
{ 0.000f, 0.000f, 0.502f },
{ 0.992f, 0.961f, 0.902f },
{ 0.502f, 0.502f, 0.000f },
{ 0.420f, 0.557f, 0.137f },
{ 1.000f, 0.647f, 0.000f },
{ 1.000f, 0.271f, 0.000f },
{ 0.855f, 0.439f, 0.839f },
{ 0.933f, 0.910f, 0.667f },
{ 0.596f, 0.984f, 0.596f },
{ 0.686f, 0.933f, 0.933f },
{ 0.859f, 0.439f, 0.576f },
{ 1.000f, 0.937f, 0.835f },
{ 1.000f, 0.855f, 0.725f },
{ 0.804f, 0.522f, 0.247f },
{ 1.000f, 0.753f, 0.796f },
{ 0.867f, 0.627f, 0.867f },
{ 0.690f, 0.878f, 0.902f },
{ 0.502f, 0.000f, 0.502f },
{ 1.000f, 0.000f, 0.000f },
{ 0.737f, 0.561f, 0.561f },
{ 0.255f, 0.412f, 0.882f },
{ 0.545f, 0.271f, 0.075f },
{ 0.980f, 0.502f, 0.447f },
{ 0.957f, 0.643f, 0.376f },
{ 0.180f, 0.545f, 0.341f },
{ 1.000f, 0.961f, 0.933f },
{ 0.627f, 0.322f, 0.176f },
{ 0.753f, 0.753f, 0.753f },
{ 0.529f, 0.808f, 0.922f },
{ 0.416f, 0.353f, 0.804f },
{ 0.439f, 0.502f, 0.565f },
{ 0.439f, 0.502f, 0.565f },
{ 1.000f, 0.980f, 0.980f },
{ 0.000f, 1.000f, 0.498f },
{ 0.275f, 0.510f, 0.706f },
{ 0.824f, 0.706f, 0.549f },
{ 0.000f, 0.502f, 0.502f },
{ 0.847f, 0.749f, 0.847f },
{ 1.000f, 0.388f, 0.278f },
{ 0.251f, 0.878f, 0.816f },
{ 0.933f, 0.510f, 0.933f },
{ 0.961f, 0.871f, 0.702f },
{ 1.000f, 1.000f, 1.000f },
{ 0.961f, 0.961f, 0.961f },
{ 1.000f, 1.000f, 0.000f },
{ 0.604f, 0.804f, 0.196f }
};

char object_names[][64] = { 
	"Hollow Sphere", "Solid Sphere", 
	"Hollow Cube", "Solid Cube", 
	"Hollow Tetrahedron", "Solid Tetrahedron" };

int object_values[6] = {0, 1, 2, 3, 4, 5};