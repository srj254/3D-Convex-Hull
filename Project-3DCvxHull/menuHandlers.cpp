#include <iostream>
#include <math.h>
#include <vector>
#include <set>
#include "gen_codes.h"
#include "Point.h"
#include "Facet.h"
#include "Halfedge.h"
#include "ConflictGraph.h"
#include "menu.h"
#include "StateObject.h"

using namespace std;

err_code draw_3dHull()
{
	unsigned 	i = 0, j = 0;
	bool		is_collinear = true;
	err_code	err_status = E_SUCCESS;
	
	if (pts.v_pts.size() < 4)
		return E_SUCCESS;

	for (i = 2; i < pts.v_pts.size(); i++)
	{
		is_collinear = check_collinear(pts.v_pts[0], pts.v_pts[1],
										pts.v_pts[i]);
		if (false == is_collinear)
		{
			std::swap(pts.v_pts[2], pts.v_pts[i]);
			break;
		}
	}
	if (true == is_collinear)
	{
		cout << "All points are collinear\n";
		exit(0);
	}
	err_status = halfedges.crt_allhalfedges();
	err_status = crt_2face_triangle(pts.v_pts[0], pts.v_pts[1], 
									pts.v_pts[2]);
	if (E_SUCCESS != err_status)
	{
		cout << "2-Face Triangle creation failed\n";
		exit(0);
	}
	pt_excl.push_back(pts.v_pts[0]);
	pt_excl.push_back(pts.v_pts[1]);
	pt_excl.push_back(pts.v_pts[2]);
	
	err_status = cnflct_graph.update(pts.v_pts, faces.v_faces, pt_excl);
	if (E_SUCCESS != err_status)
	{
		cout << "Conflict graph update failed\n";
		exit(0);
	}

	{
		StateObject		S;
		S.store_faces(faces.v_faces, false);
	}

	for (i = 3; i < pts.v_pts.size(); i++)
	{
		vector<Facet>		cnflct_faces;
		vector<Halfedge>	horizon_edges;
		vector<int>			facet_edges;
		Pt					p = pts.v_pts[i];
		
		cout << "========== " << i << " ========= " << endl;
		
		{
			StateObject		S;
			S.store_faces(faces.v_faces, false);
			states.add_state(S);
		}

		for (j = 0; j < faces.v_faces.size(); j++)
				if (E_YES_CNFLCT == faces.v_faces[j].get_conflict_state(i))	
					cnflct_faces.push_back(faces.v_faces[j]);
		
		if (cnflct_faces.size() == 0)
			cout << "No conflicting faces" << endl;

		for (j = 0; j < cnflct_faces.size(); j++)
		{
			Halfedge		H(cnflct_faces[j].getboundary());
			Pt				o(*H.getorigin());
			Facet			*f_ptr = nullptr;
			int				face_ID;

			do 
			{
				face_ID = (H.gettwin())->getface();
				if (-1 != faces.findface(face_ID))
					f_ptr = &(faces.v_faces.at(faces.findface(face_ID)));
				else
					exit(0);
				if (E_NO_CNFLCT == f_ptr->get_conflict_state(i))
					horizon_edges.push_back(H);
				H = *(H.getnext()); // Move to next edge of facet
			} while (!(o == *H.getorigin())); // Until the starting point
		}

		{
			StateObject		S;
			S.store_faces(cnflct_faces, true);

			for (j = 0; j < cnflct_faces.size(); j++)
				faces.removeFace(cnflct_faces[j]);
			
			S.store_faces(faces.v_faces, false);
			states.add_state(S);
		}

		int hedge_pos = -1;
		for (j = 0; j < horizon_edges.size(); j++)
		{
			hedge_pos = -1;
			facet_edges.clear();

			hedge_pos = halfedges.find_halfedge(pts.v_pts[i], 
								*horizon_edges[j].getorigin());
			facet_edges.push_back(hedge_pos);

			hedge_pos = halfedges.find_halfedge(
									*horizon_edges[j].getorigin(), 
									*horizon_edges[j].getdest());
			facet_edges.push_back(hedge_pos);

			hedge_pos = halfedges.find_halfedge(
						*(horizon_edges[j].gettwin())->getorigin(),
						pts.v_pts[i]);

			facet_edges.push_back(hedge_pos);

			try 
			{
				Facet	F1(facet_edges, pts.v_pts.size(), -1);
				faces.addFace(F1);
			}
			catch (err_code &e) 
			{
				cout << "Failure to create triangle fan: " << e << endl;
				exit(0);
			}
		}

		pt_excl.push_back(p);
		err_status = cnflct_graph.update(pts.v_pts, faces.v_faces, 
										 pt_excl);
	}
	{
		StateObject		S;
		S.store_faces(faces.v_faces, false);
		states.add_state(S);
	}

	cout << endl << "States: " << states.v_stateObjects.size() << endl;
	return E_SUCCESS;
}
