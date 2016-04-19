#pragma once

#include <iostream>
#include "gen_codes.h"
#include "Point.h"

class Facet;

class Halfedge {
	private:
		int			ID;
		Pt			*origin;
		Pt			*dest;
		Halfedge	*twin;
		Halfedge	*next;
		Halfedge	*prev;
		int			face_ID;
	public: 
		Halfedge()
		{
			ID = 0;
			origin = nullptr;
			dest = nullptr;
			twin = nullptr;
			next = nullptr;
			prev = nullptr;
			face_ID = -1;
		}
		Halfedge(Pt *o, Pt *d);
		Halfedge(const Halfedge &other)
		{
			ID = other.ID;
			origin = other.origin;
			dest = other.dest;
			twin = other.twin;
			next = other.next;
			prev = other.prev;
			face_ID = other.face_ID;
		}
		inline void setID(int n)
		{ 
			ID = n;
		}
		inline int  getID()
		{ 
			return ID;
		}
		Pt*		  getorigin();
		Pt*		  getdest();
		Halfedge* gettwin();
		Halfedge* getnext();
		Halfedge* getprev();
		int		  getface();

		err_code  settwin(Halfedge &H);
		err_code  setnext(Halfedge &H);
		err_code  setprev(Halfedge &H);
		err_code  setface(Facet	   &f);
		void	  resetface();

		Halfedge& operator=(const Halfedge &other)
		{
			ID	= other.ID;
			origin = other.origin;
			dest = other.dest;
			twin = other.twin;
			next = other.next;
			prev = other.prev;
			face_ID = other.face_ID;
			return (*this);
		}
		bool operator==(const Halfedge &other)
		{
			if ((other.dest == dest) && (other.origin == origin))
				return true;
			else
				return false;
		}
		void print_halfedge()
		{
			cout << "Halfedge: ";
			origin->print_point();
			cout << " ->"; 
			dest->print_point();
		}

};

class allHalfedges
{
	public:
		vector<Halfedge>	v_halfedges;

		allHalfedges()
		{
			v_halfedges.clear();
		}
		err_code crt_allhalfedges();
		int find_halfedge(Pt &o, Pt &d);
		void allHalfedges::clearall()
		{
			v_halfedges.clear();
		}
};

extern allHalfedges halfedges;