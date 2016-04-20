#pragma once

#include <iostream>
#include <vector>
#include <set>
#include "gen_codes.h"
#include "Point.h"

typedef struct polygon_t
{
	unsigned int	vertices[3];
}polygon_t;

typedef struct edge_t
{
	unsigned int	vertices[2];
};

class StateObject
{
public:
	vector<polygon_t>	faces;
	vector<polygon_t>	remove_faces;
	vector<polygon_t>	color;
	vector<edge_t>		edges;

	int					highlight_pt;
	vector<int>			interior_pts;
	//vector<int>			hull_pts;
	vector<int>			exterior_pts;

public:
	StateObject()
	{
		highlight_pt = -1;
	}
	bool store_faces(vector<Facet> &faces, bool delete_face);
	bool set_highlight_pt(int v_index);
	bool set_exterior_pts();
};

class allstates
{

	public:
	vector<StateObject>	v_stateObjects;

	bool add_state(StateObject &S)
	{
		v_stateObjects.push_back(S);
		return true;
	}
};

extern allstates		states;
