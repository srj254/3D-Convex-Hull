#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <set>

#include "gen_codes.h"
#include "Point.h"
#include "ConflictGraph.h"

err_code ConflictGraph::update(vector<Pt>&	pt_vector,
						       vector<Facet>& face_vector,
							   vector<Pt>&	pt_excl)
{
	unsigned int		i = 0, j = 0;
	bool				e_pt_excl = false;
	pt_orient			e_orient = E_ABOVE;

	try {
		for (j = 0; j < face_vector.size(); j++)
		{
			for (i = 0; i < pt_vector.size(); i++)
			{
				if (E_NO_CNFLCT == face_vector[j].pt_face_cnflct[i])
					continue;

				if (pt_excl.end() != find(pt_excl.begin(), pt_excl.end(),
					pt_vector[i]))
				{
					face_vector[j].pt_face_cnflct[i] = E_NO_CNFLCT;
				}
				else
				{
					try {
						e_orient = face_vector[j].eval_point(pt_vector[i]);
						if (E_BELOW == e_orient)
							face_vector[j].pt_face_cnflct[i] = E_NO_CNFLCT;
						else
							face_vector[j].pt_face_cnflct[i] = E_YES_CNFLCT;
					}
					catch (exception &e)
					{
						cout << "Error Here";
					}
				}
			}
		}
	}
	catch (exception &e)
	{
		cout << "Exception " << e.what() << endl;
	}
	return E_SUCCESS;
}

