#pragma once

#include <iostream>
#include <vector>
#include <set>
#include "gen_codes.h"
#include "Point.h"

typedef struct polygon_t
{
	int		vertices[MAX_POINTS];
}polygon_t;

typedef struct StateObject
{
	vector<polygon_t>	polygons;
	vector<polygon_t>	interior_polygon;
	
	vector<int>			highlight_pt;
	vector<int>			interior_pts;
	vector<int>			hull_pts;
	vector<int>			exterior_pts;

}StateObject;

extern StateObject		*state_objects;
