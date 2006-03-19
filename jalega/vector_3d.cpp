#include "vector_3d.hpp"

using namespace std;

Vector_3D::Vector_3D ():
	Object::Object ("Vector_3D")
{
	clog << "Warning: vector unexplicitely created! " << endl;
	x = 0;
	y = 0;
}

Vector_3D::Vector_3D (int new_x , int new_y, int new_z):
	Object::Object ("Vector_3D")
{
	x = new_x;
	y = new_y;
	z = new_z;
}

Vector_3D::~Vector_3D ()
{
/// Empty!
}

int Vector_3D::length () const
{
	return sqrt (x * x + y * y + z * z);
}

int Vector_3D::distance (Vector_3D other_vector) const
{
    Vector_3D diff = * this - other_vector;

	return diff.length();
}

string Vector_3D::to_String() const
{
	return "(" + int_To_Str(x) + ", " + int_To_Str(y) + ")";
}

void &Vector_3D::operator=(const Vector_3D &v)
{
	x = v.x;
	y = v.y;
}

Vector_3D Vector_3D::half() const
{
	return Vector_2D(x / 2, y / 2);
}

Vector_3D Vector_3D::absolut() const
{
	return Vector_2D(abs(x), abs(y));
}

Vector_2D operator-(const Vector_3D &v)
{
	return Vector_2D(-v.x , -v.y);
}

Vector_2D swap_Coordinates(const Vector_3D &v)
{
	return Vector_2D(v.y , v.x);
}

Vector_2D operator+(const Vector_2D &a, const Vector_2D &b)
{
	return Vector_2D(a.x + b.x , a.y + b.y);
}

Vector_2D operator-(const Vector_2D &a, const Vector_2D &b)
{
	return Vector_2D(a.x - b.x , a.y - b.y);
}

Vector_2D multiply_Coordinates(const Vector_2D &a, const Vector_2D &b)
{
	return Vector_2D(a.x * b.x , a.y * b.y);
}

Vector_2D divide_Coordinates(const Vector_2D &a, const Vector_2D &b)
{
	return Vector_2D(a.x / b.x , a.y / b.y);
}

const Vector_2D &operator+=(Vector_2D &a, const Vector_2D &b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

bool operator==(const Vector_2D &a, const Vector_2D &b)
{
	return (a.x == b.x) && (a.y == b.y);
}

ostream &operator<<(ostream &os, const Vector_2D &v)
{
	os << v.to_String();
	return os;
}
