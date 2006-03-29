#include "vector_3d.hpp"

using namespace std;

Vector_3D::Vector_3D (float new_x , float new_y, float new_z):
	Object::Object ("Vector_3D")
{
	x = new_x;
	y = new_y;
	z = new_z;
}

Vector_3D::~Vector_3D ()
{
}

double Vector_3D::length () const
{
	return sqrt ( x * x + y * y + z * z);
}

double Vector_3D::distance (const Vector_3D &b) const
{
    Vector_3D diff = * const_cast <Vector_3D *> (this) - b;

	return diff.length ();
}

void Vector_3D::operator= (const Vector_3D &b)
{
	x = b.x;
	y = b.y;
}

Vector_3D Vector_3D::operator- () const
{
	return Vector_3D (- x , - y, - z);
}

Vector_3D Vector_3D::operator* (float a) const
{
	return Vector_3D (a * x , a * y, a * z);
}

Vector_3D Vector_3D::operator/ (float a) const
{
	return Vector_3D (x / a,  y / a, z / a);
}

Vector_3D Vector_3D::operator+ (const Vector_3D &b)
{
	return Vector_3D (x + b.x , y + b.y, z + b.z);
}

Vector_3D Vector_3D::operator- (const Vector_3D &b)
{
	return Vector_3D (x - b.x , y - b.y, z - b.z);
}

void Vector_3D::operator+= (const Vector_3D &b)
{
	x += b.x;
	y += b.y;
	z += b.z;
}

bool Vector_3D::operator== (const Vector_3D &b)
{
	return (x == b.x) && (y == b.y) && (z == b.z);
}
