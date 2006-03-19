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
		Vector_3D (const Vector_3D &vector);
		virtual ~Vector_3D ();
		
		int length () const;
		int distance (Vector_3D other_vector) const;
		Vector_3D swap_Coordinates () const;
		Vector_3D half () const;
		const Vector_3D &operator= (const Vector_3D &other_vector);

		int x , y, z;
};

Vector_3D operator- (const Vector_3D &other_vector);

Vector_3D operator+ (const Vector_3D &a, const Vector_3D &b);
Vector_3D operator- (const Vector_3D &a, const Vector_3D &b);
/// No '*' and '/', they do not make sense.
Vector_3D multiply_Coordinates (const Vector_3D &a, const Vector_3D &b);
Vector_3D divide_Coordinates (const Vector_3D &a, const Vector_3D &b);

const Vector_3D &operator+= (Vector_3D &a, const Vector_3D &b);

bool operator== (const Vector_3D &a , const Vector_3D &b);

ostream &operator<< (ostream &os , const Vector_3D &other_vector);

#endif
