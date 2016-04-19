#pragma once

#include <iostream>
#include "gen_codes.h"
#include "Point.h"
#include "Facet.h"

class ConflictGraph
{
	public:
		err_code update(vector<Pt> &pt_vector, vector<Facet> &face_vector, vector<Pt> &pt_excl);
};

extern ConflictGraph	cnflct_graph;
