#include <iostream>
#include "Facet.h"
#include "ConflictGraph.h"
#include "StateObject.h"

bool StateObject::store_faces(vector<Facet>& in_faces, bool delete_face,
								bool splcolor_lastface)
{
	polygon_t		p = { 0 };
	edge_t			e = { 0 };	

	this->has_new_face = false;
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
		{
			if ((i == in_faces.size() - 1) && splcolor_lastface)
			{	
				this->new_face.vertices[0] = p.vertices[0];
				this->new_face.vertices[1] = p.vertices[1];
				this->new_face.vertices[2] = p.vertices[2];
				this->has_new_face = true;
			}
			else
				this->polygon_faces.push_back(p);
		}
		else
		{
			this->remove_faces.push_back(p);
		}
	}
	return true;
}

bool StateObject::set_highlight_pt(int v_index)
{
	this->highlight_pt = v_index;
	this->set_exterior_pts();
	return true;
}

bool StateObject::set_exterior_pts(bool checkhull)
{
	for (unsigned i = 0; i < pts.v_pts.size(); i++)
	{
		if (i != highlight_pt)
		{
			if (std::find(pt_excl.begin(), pt_excl.end(), pts.v_pts.at(i))
				!= pt_excl.end())
			{
				if (!checkhull)
					this->hull_pts.push_back(i);
				else
				{
					if (std::find(hull_ptIndex.begin(), hull_ptIndex.end(), i)
						!= hull_ptIndex.end())
					{
						this->hull_pts.push_back(i);
					}
					else
						this->interior_pts.push_back(i);

				}
			}
			else
			{
				bool	state = true;
				for (unsigned j = 0; j < faces.v_faces.size(); j++)
					if (faces.v_faces.at(j).get_conflict_state(i) == E_YES_CNFLCT)
						state = false;
				if (state)
					this->interior_pts.push_back(i);
				else 
					this->exterior_pts.push_back(i);
			}
		}
	}
	
	return true;
}

bool StateObject::store_horizon(vector<Halfedge>	&horizon_edges)
{
	edge_t			e = { 0 };
	for (unsigned i = 0; i < horizon_edges.size(); i++)
	{
		e.vertices[0] = horizon_edges.at(i).getorigin()->getID();
		e.vertices[1] = horizon_edges.at(i).getdest()->getID();
		this->horizon_edges.push_back(e);
	}
	//cout << "Horizon Edges size:" << horizon_edges.size()<< endl;
	return true;
}