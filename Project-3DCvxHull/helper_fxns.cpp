#include <iostream>
#include <math.h>
#include <set>
#include "gen_codes.h"
#include "Point.h"
#include "Facet.h"
#include "Halfedge.h"
#include "ConflictGraph.h"
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
		pointArray[j] = px; j++;
		pointArray[j] = py; j++;
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

void getFacesInArray()
{
	int		j = 0, k = 0;
	
	nFaces = 0;
	nHEdges = 0;

	for (unsigned i = 0; i < faces.v_faces.size(); i++)
	{
		Facet		*f = &(faces.v_faces.at(i));
		Pt			o(*(f->getboundary().getorigin()));
		Halfedge	H(f->getboundary());
		unsigned	count = 0;
		
		do
		{
			hullEdges[j] = H.getorigin()->getID(); j++;
			hullEdges[j] = H.getdest()->getID(); j++;
			nHEdges++; count++;
			hullFaces[k] = H.getorigin()->getID(); k++;
			H = (*H.getnext());
		} while (!(o == (*H.getorigin())));
		
		if (count > 3)
		{
			cout << "Face edges are more than 3";
			getchar();
		}
		nFaces++;
	}

	if (false) //(1 == fchange)
	{
		cout << "\nFaces: " << nFaces << endl;
		for (int ii = 0; ii < nFaces * 3; ii += 3)
			cout << hullFaces[ii] << "," << 
					hullFaces[ii + 1] << "," << hullFaces[ii + 2] << endl;
		cout << "Halfedges: " << nHEdges << endl;
		for (int ii = 0; ii < nHEdges * 2; ii += 2)
			cout << hullEdges[ii] << "," << hullEdges[ii + 1] << endl;

		fchange = 0;
	}
	return;
}