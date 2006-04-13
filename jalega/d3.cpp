#include "d3.hpp"

using namespace std;

D3::D3 (float new_x , float new_y, float new_z):
	Object::Object ("D3")
{
	assert (Object::is_initialized ());
	
	x = new_x;
	y = new_y;
	z = new_z;
	
	assert (is_initialized ());
}

D3::~D3 ()
{
	assert (is_initialized ());
}

double D3::length () const
{
	assert (is_initialized ());
	
	return sqrt ( x * x + y * y + z * z);
}

double D3::distance (const D3 &b) const
{
	assert (is_initialized ());
	
	D3 diff = * const_cast <D3 *> (this) - b;
	return diff.length ();
}

D3 D3::operator- () const
{
	assert (is_initialized ());
	
	return D3 (- x , - y, - z);
}

D3 D3::operator* (float a) const
{
	assert (is_initialized ());
	
	return D3 (a * x , a * y, a * z);
}

D3 D3::operator/ (float a) const
{
	assert (is_initialized ());
	
	return D3 (x / a,  y / a, z / a);
}

D3 D3::operator+ (const D3 &b)
{
	assert (is_initialized ());
	
	return D3 (x + b.x , y + b.y, z + b.z);
}

D3 D3::operator- (const D3 &b)
{
	assert (is_initialized ());
	
	return D3 (x - b.x , y - b.y, z - b.z);
}

void D3::operator+= (const D3 &b)
{
	assert (is_initialized ());
	
	x += b.x;
	y += b.y;
	z += b.z;
}

bool D3::operator== (const D3 &b) const
{
	assert (is_initialized ());
	
	return (x == b.x) && (y == b.y) && (z == b.z);
}
