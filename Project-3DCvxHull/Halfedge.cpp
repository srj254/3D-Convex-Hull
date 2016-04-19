#include <iostream>
#include <math.h>
#include <set>
#include "gen_codes.h"
#include "Point.h"
#include "Facet.h"
#include "Halfedge.h"
#include "menu.h"

Halfedge::Halfedge(Pt *o, Pt *d)
{
	ID = -1;
	origin = o;
	//cout << origin << endl;
	dest = d;
	//cout << dest << endl;
	next = twin = prev = nullptr;
	face_ID = -1;
}

Pt* Halfedge::getorigin()
{
	//cout << origin << endl;
	return origin;
}

Pt* Halfedge::getdest()
{
	return dest;
}

Halfedge* Halfedge::gettwin()
{
	if (twin == nullptr)
	{
		cout << "returning NULL" << endl;
		
	}
	return twin;
}

Halfedge* Halfedge::getnext()
{
	return next;
}

Halfedge * Halfedge::getprev()
{
	return prev;
}

int Halfedge::getface()
{
	return face_ID;
}

err_code Halfedge::settwin(Halfedge &H)
{
	twin = (Halfedge*) &H;
	//cout << twin << endl;
	return E_SUCCESS;
}

err_code Halfedge::setnext(Halfedge &H)
{
	next = (Halfedge*)&H;
	return E_SUCCESS;
}

err_code Halfedge::setprev(Halfedge &H)
{
	prev = (Halfedge*)&H;
	return E_SUCCESS;
}

err_code Halfedge::setface(Facet &f)
{
	face_ID = f.getID();
	return E_SUCCESS;
}

void Halfedge::resetface()
{
	face_ID = -1;
}

err_code allHalfedges::crt_allhalfedges()
{
	unsigned int i = 0, j = 0;
	unsigned int n = 0;
	unsigned int index = 0;
	//cout << pts.v_pts.size() << endl;
	v_halfedges.clear();
	v_halfedges.reserve((pts.v_pts.size()*(pts.v_pts.size()-1)));
	//cout << v_halfedges.capacity() << endl;

	for (i = 0; i < pts.v_pts.size(); i++)
	{	
		for (j = i + 1; j < pts.v_pts.size(); j++)
		{
			//cout << i << j << endl;
			Halfedge	H(&pts.v_pts[i], &pts.v_pts[j]);
			Halfedge	Htwin(&pts.v_pts[j], &pts.v_pts[i]);

			//cout << "H_Edge: ";
			//(H.getorigin())->print_point();
			//cout << ", ";
			//(Htwin.getorigin())->print_point();
			//cout << endl;

			H.setID(n); n++;
			Htwin.setID(n); n++;

			v_halfedges.push_back(H);
			index = v_halfedges.size(); // index of twin
			v_halfedges.push_back(Htwin);
			//cout << "Index:" << index;
			//cout << "Size: " << v_halfedges.size() << endl;
			//getchar();

			v_halfedges[index].settwin(v_halfedges[index - 1]);
			v_halfedges[index-1].settwin(v_halfedges[index]);
			//for (unsigned k = 0; k < v_halfedges.size(); k++)
			//{
			//	cout << k << "\t: O -> O(TW) :\t";
			//	v_halfedges[k].getorigin()->print_point();
			//	cout << "\t";
			//	v_halfedges[k].gettwin()->getorigin()->print_point();
			//	cout << endl;
			//}

			//Halfedge &ref1 = v_halfedges.back();

			//Halfedge &ref2 = v_halfedges.back();
			//
			//ref1.settwin(ref2); // Set the twin pointers
			//ref2.settwin(ref1);
			//cout << &ref1 << &ref2 << ref1.getID() << ref2.getID();

			//cout << "H_Edge: ";
			//(ref1.getorigin())->print_point();
			//cout << ", ";
			//(ref2.getorigin())->print_point();
			//cout << endl;
		}
		//cout << "I: "<< i << endl;
		//getchar();
	}

	//cout << "Total Halfedges are:" << v_halfedges.size();
	//getchar();

	//for (i = 0; i < v_halfedges.size(); i++)
	//{
	//	cout << "H_Edge" << i << ":";  
	//	v_halfedges[i].getorigin()->print_point();
	//	cout << ", ";
	//	v_halfedges[i].getdest()->print_point();
	//	cout << endl;
	//	if (i > 50) 
	//		getchar();
	//}
	return E_SUCCESS;
}

int allHalfedges::find_halfedge(Pt &o, Pt &d)
{
	try 
	{
		for (unsigned i = 0; i < v_halfedges.size(); i++)
			if ((o) == (*v_halfedges[i].getorigin()))
				if (d == *(v_halfedges[i].getdest()))
					return i;
	}
	catch (exception &e)
	{
		cout << "Exception was caught here " << e.what() << endl;
	}
	return -1;
}
