#ifndef VECTOR_3D_HPP
#define VECTOR_3D_HPP

#include <cstdlib>
#include <cmath>
#include "object.hpp"

using namespace std;

/// This represent a vector in a two dimensional plane.
class Vector_3D:
	public Object
{
    public:
		Vector_3D ();
		Vector_3D (int new_x , int new_y, int new_z);
		Vector_3D (const Vector_3D &b);
		virtual ~Vector_3D ();
		
		double length () const;
		double distance (const Vector_3D &b) const;
		Vector_3D half () const;
		Vector_3D operator- () const;
		Vector_3D operator* (int a) const;
		Vector_3D operator/ (int a) const;
		Vector_3D operator+ (const Vector_3D &b);
		Vector_3D operator- (const Vector_3D &b);
		void operator= (const Vector_3D &b);
		void operator+= (const Vector_3D &b);
		bool operator== (const Vector_3D &b);
		
		int x , y, z;
};

#endif
