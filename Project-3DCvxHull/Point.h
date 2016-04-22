#pragma once
#include <vector>
#include <set>

using namespace std;

#define		MAX_POINTS		1000
#define		MAX_FACES		3000
#define		MAX_HALFEDGES	(MAX_POINTS * MAX_POINTS)

class Halfedge;
class Pt
{
	private:
		int ID;
		float x;
		float y;
		float z;
	public:
		bool operator==(const Pt &rhs);
		friend class ConflictGraph;

		Pt(float x=0, float y=0, float z=0, int n=0);
		Pt(const Pt &p, int x_offset, int y_offset, int z_offset, 
		   int n = 0); // Unused currently

		inline int  getID()
		{
			return ID;
		}
		inline void setID(int newID)
		{
			ID = newID;
		}

		float X();
		float Y();
		float Z();
		err_code set_point(float x, float y, float z);
		inline void print_point() 
		{ 
			if (this == NULL)
				cout << "This is null" << endl;
			else
				cout << ID;//printf("(%f, %f, %f)", x, y, z); 
		}
		inline void print_point_verbose()
		{
			if (this == NULL)
				cout << "This is null" << endl;
			else
				printf("(%f, %f, %f)", x, y, z); 
		}

};

typedef enum point_type
{
	E_RANDOM
}point_type;

class Points
{
	public:
		vector<Pt>	v_pts;
	
		Points()
		{}
		err_code loadpoints(FILE *fp);
		int		 noofpoints();
		void	 clearpoints();
		void	 generate_points(int lb, int ub, point_type typ, int nPts);
};

extern Points	pts;
/* Hold all the points considered */
extern vector<Pt>	pt_excl;

