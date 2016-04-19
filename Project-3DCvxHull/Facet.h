#pragma once

#include <iostream>
#include "gen_codes.h"
#include "Halfedge.h"
#include "Point.h"

class Facet
{
	private: 
		int						ID;
		Halfedge			    *boundary_edge;
		vector<conflict_code>	pt_face_cnflct;
		static int				next_ID;
	public:

		friend class ConflictGraph;

		Facet(vector<int> hf_edge_vctr, int n, int ID);
		//Facet(const Facet &f);

		inline int  getID()
		{
			return ID;
		}

		pt_orient eval_point(Pt &p);
		inline Halfedge& getboundary()
		{
			return (*boundary_edge);
		}
		conflict_code	get_conflict_state(int i);
		bool			operator==(const Facet &F);
};

class allFaces
{
	private:
		int				n; // Next ID number
						   // Doesn't indicate faces

	public:
		vector<Facet>	v_faces;
	
		allFaces()
		{
			v_faces.clear();
			n = 0;
		}
		int			nooffacets();
		int			findface(Facet &f);
		int			findface(int face_ID);
		err_code	addFace(Facet &f);
		err_code	removeFace(Facet &f);
		void allFaces::clearall()
		{
			this->n = 0;
			this->v_faces.clear();
		}
};

extern allFaces faces;
