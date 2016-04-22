#include <iostream>
#include <algorithm>
#include <math.h>

#include "gen_codes.h"
#include "Point.h"
#include "Facet.h"

using namespace std;

int				Facet::next_ID = 0;
Facet::Facet(vector<int> hf_edge_vctr, int n, int id)
{
	unsigned 	i = 0;
	Pt			o(*(halfedges.v_halfedges.
					at(hf_edge_vctr.at(i)).getorigin()));

	this->pt_face_cnflct.resize(n, E_NEW);
	boundary_edge = nullptr;
	
	this->ID = Facet::next_ID;
	Facet::next_ID++;

	for (i = 0; i < hf_edge_vctr.size(); i++)
	{
		if (o == *(halfedges.v_halfedges.at(hf_edge_vctr.at(i))).getdest())
		{
			//cout << "Equal points" << endl;
			break;
		}
	}
	if (i != (hf_edge_vctr.size() - 1))
	{
		//cout << "I'm failing " << i << endl;
		throw E_FAILURE;
	}
	else
	{
		int local_index = 0, next_local_index = 0, prev_local_index = 0;
		i = 0;
		boundary_edge = &(halfedges.v_halfedges.at(hf_edge_vctr.at(i)));
		for (i = 0; i < hf_edge_vctr.size(); i++)
		{
			local_index = hf_edge_vctr.at(i);
			next_local_index = hf_edge_vctr.at((i + 1) % hf_edge_vctr.size());
			prev_local_index = hf_edge_vctr.at((i - 1 + hf_edge_vctr.size()) 
								% hf_edge_vctr.size());
			(halfedges.v_halfedges.at(local_index)).
				setnext(halfedges.v_halfedges.at(next_local_index));
			(halfedges.v_halfedges.at(local_index)).
				setprev(halfedges.v_halfedges.at(prev_local_index));
			(halfedges.v_halfedges.at(local_index)).setface(*this);
		}

	}
}

pt_orient Facet::eval_point(Pt &p)
{
	double	a = 0, b = 0, c = 0, d = 0;
	double	distance = 0;
	Pt		A, B, C;

	A = *boundary_edge->getorigin();
	B = *boundary_edge->getnext()->getorigin();
	C = *boundary_edge->getprev()->getorigin();

	a = ((B.Y() - A.Y()) * (C.Z() - A.Z())) -
		((C.Y() - A.Y()) * (B.Z() - A.Z()));

	b = ((B.Z() - A.Z()) * (C.X() - A.X())) -
		((C.Z() - A.Z()) * (B.X() - A.X()));

	c = ((B.X() - A.X()) * (C.Y() - A.Y())) -
		((C.X() - A.X()) * (B.Y() - A.Y()));

	d = -(a * A.X() + b * A.Y() + c * A.Z());

	distance =	(a* p.X() + b * p.Y() + c * p.Z() + d) /
				(sqrt(a*a + b*b + c*c));

	if (distance > (0 - 0.0001) && distance < (0 + 0.0001))
	{
		return E_EQUAL;
	}
	else if (distance < 0)
	{
		return E_BELOW;
	}
	else 
		return E_ABOVE;
}

conflict_code Facet::get_conflict_state(int i)
{
	return this->pt_face_cnflct.at(i);
}

bool Facet::operator==(const Facet & F)
{
	Halfedge	*H = boundary_edge;

	do
	{
		if (*H == *(F.boundary_edge))
			return true;
		H = H->getnext();
	} while(H != boundary_edge);

	return false;
}

int allFaces::findface(Facet &f)
{
	unsigned		i = 0;
	for (i = 0; i < v_faces.size(); i++)
	{
		if (f == v_faces[i])
			return i;
	}
	return -1;
}

int allFaces::findface(int face_ID)
{
	unsigned		i = 0;
	for (i = 0; i < v_faces.size(); i++)
	{
		if (face_ID == v_faces[i].getID())
			return i;
	}
	return -1;
}

int allFaces::nooffacets()
{
	return v_faces.size();
}

err_code allFaces::addFace(Facet & f)
{
	this->n++;
	v_faces.push_back(f);
#if DEBUG
	cout << "Add " << f.getID() << " Size: " << v_faces.size() << endl;
#endif
	return E_SUCCESS;
}

err_code allFaces::removeFace(Facet & f)
{
	v_faces.erase(std::remove(v_faces.begin(), v_faces.end(), f),
				  v_faces.end());
#if DEBUG
	cout << "Remove NFaces:" << v_faces.size() << " List: ";
	for (unsigned i = 0; i < v_faces.size(); i++)
		cout << v_faces.at(i).getID() << " ";
	cout << endl;
#endif
	return E_SUCCESS;
}
