#include <iostream>
#include <iterator>
#include <vector>
#include <set>
#include <math.h>

#include "gen_codes.h"
#include "Point.h"

using namespace std;

Pt::Pt(float x, float y, float z, int n)
{
	ID = pts.v_pts.size();
	this->x = x;
	this->y = y;
	this->z = z;
}

Pt::Pt(const Pt & p, int x_offset, int y_offset, int z_offset, int n)
{
	ID = pts.v_pts.size();
	this->x = p.x + x_offset;
	this->y = p.y + y_offset;
	this->z = p.z + z_offset;
}

float Pt::X()
{
	return this->x;
}
float Pt::Y()
{
	return this->y;
}
float Pt::Z()
{
	return this->z;
}

err_code Pt::set_point(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;

	return E_SUCCESS;
}

int Points::noofpoints()
{
	return v_pts.size();
}

inline bool isvalid(float input, float lb, float ub)
{
	if (input < lb || input > ub)
		return false;
	return true;
}

err_code Points::loadpoints(FILE * fp)
{
	char	readline[512] = { 0 };
	int		input_pts = 0;
	float	px, py, pz;
		
	fscanf_s(fp, "%d\n", &input_pts);
	if (input_pts > 40)
	{
		cout << "Number of points should be within 40";
		return E_FAILURE;
	}
	while ((!feof(fp)) && (input_pts > 0))
	{
		fscanf_s(fp, "%f %f %f\n", &px, &py, &pz);
		if (!(isvalid(px, -1, 1) && isvalid(py, -1, 1) && isvalid(pz, -1, 1)))
			continue;
		
		Pt	P(px, py, pz, this->v_pts.size());
		this->v_pts.push_back(P);
		input_pts--;
	}
	
	for (unsigned i = 0; i < v_pts.size(); i++)
	{
		cout << v_pts[i].getID() << " ";  v_pts[i].print_point_verbose();
		cout << endl;
	}
	return E_SUCCESS;
}

void Points::clearpoints()
{
	v_pts.clear();
}

void Points::generate_points(int lb, int ub, point_type typ, int nPts)
{
	switch (typ)
	{
		case E_RANDOM:
		{
			break;
		}
	}
}

bool Pt::operator==(const Pt &rhs)
{
	if ((x == rhs.x) && (y == rhs.y) && (z == rhs.z))
	{
		return true;
	}
	return false;
}