#include <iostream>
#include "Facet.h"
#include "StateObject.h"

bool StateObject::store_faces(vector<Facet>& in_faces, bool delete_face)
{
	polygon_t		p = { 0 };
	edge_t			e = { 0 };	

	for (unsigned i = 0; i < in_faces.size(); i++)
	{
		Facet		*f = &(in_faces.at(i));
		Pt			o(*(f->getboundary().getorigin()));
		Halfedge	H(f->getboundary());
		int			k = 0;
		do
		{
			try {
				if (k == 3)
					throw "Polygon is not a Triangle!!!";
			}
			catch (char *e)
			{
				cout << e << endl;
				getchar();
				exit(0);
			}

			e.vertices[0] = H.getorigin()->getID();
			e.vertices[1] = H.getdest()->getID();
			this->edges.push_back(e);
			p.vertices[k] = H.getorigin()->getID(); k++;
			H = (*H.getnext());
		} while (!(o == (*H.getorigin())));
		
		if (!delete_face)
			this->faces.push_back(p);
		else
			this->remove_faces.push_back(p);
	}
	return true;
}

bool StateObject::set_highlight_pt(int v_index)
{
	this->highlight_pt = v_index;
	return true;
}

bool StateObject::store_other_pts()
{
	// Call Conflict Graph APIS to extract all the 
	// considered and non considered points
	return false;
}
