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

	if (false) //1 == pchange)
	{
		cout << "Points: " << nPts << endl;
		for (unsigned c = 0; c < pts.v_pts.size(); c++)
			pts.v_pts.at(c).print_point();
		pchange = 0;
	}
	return;
}

void init_norm_face_color()
{
	for (unsigned i = 0; i < pts.v_pts.size(); i++)
	{
		ptColors[i * 4 + 0] = 0.5;
		ptColors[i * 4 + 1] = 0.5;
		ptColors[i * 4 + 2] = 0.0;
		ptColors[i * 4 + 3] = 0.7;
	}
}

void init_remv_face_color()
{
	for (unsigned i = 0; i < pts.v_pts.size(); i++)
	{
		ptColors[i * 4 + 0] = 0.2;
		ptColors[i * 4 + 1] = 0.2;
		ptColors[i * 4 + 2] = 0.2;
		ptColors[i * 4 + 3] = 0.7;
	}
}

void init_norm_line_color()
{
	for (unsigned i = 0; i < pts.v_pts.size(); i++)
	{
		ptColors[i * 4 + 0] = 0.0;
		ptColors[i * 4 + 1] = 0.0;
		ptColors[i * 4 + 2] = 0.0;
		ptColors[i * 4 + 3] = 0.5;
	}
}

void spl_point_colors(StateObject &S)
{
	for (unsigned i = 0; i < pts.v_pts.size(); i++)
	{
		if (i == S.highlight_pt)
		{
			ptColors[i * 4 + 0] = 0.0;
			ptColors[i * 4 + 1] = 0.0;
			ptColors[i * 4 + 2] = 1.0; // Blue
			ptColors[i * 4 + 3] = 0.5;
		}
		else if (std::find( S.exterior_pts.begin(), 
							S.exterior_pts.end(), i) != 
							S.exterior_pts.end())
		{
			ptColors[i * 4 + 0] = 1.0; // Red
			ptColors[i * 4 + 1] = 0.0;
			ptColors[i * 4 + 2] = 0.0;
			ptColors[i * 4 + 3] = 0.5;
		}
		else
		{
			ptColors[i * 4 + 0] = 0.0;
			ptColors[i * 4 + 1] = 1.0; // Red
			ptColors[i * 4 + 2] = 0.0;
			ptColors[i * 4 + 3] = 0.5;
		}
	}
}

void init_point_colors()
{
	for (unsigned i = 0; i < pts.v_pts.size(); i++)
	{
		ptColors[i * 4 + 0] = 0.0;
		ptColors[i * 4 + 1] = 0.0;
		ptColors[i * 4 + 2] = 0.0;
		ptColors[i * 4 + 3] = 0.5;
	}
}