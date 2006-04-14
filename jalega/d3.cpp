#include "d3.hpp"

using namespace std;

D3::
	D3
	(float new_x , float new_y, float new_z):
	Object::
	Object
	("D3")
{
	assert (Object::is_initialized ());
	
	x = new_x;
	y = new_y;
	z = new_z;
	
	assert (is_initialized ());
}

D3::
	D3
	(string new_name, D3 * b):
	Object::
	Object
	(new_name)
{
	assert (Object::is_initialized ());
	assert (b != NULL);
	assert (b->is_initialized ());

	x = b->get_x ();
	y = b->get_y ();
	z = b->get_z ();

	delete b;
	
	assert (is_initialized ());
}

D3::
	~D3 ()
{
	assert (is_initialized ());
}

float D3::length () const
{
	assert (is_initialized ());
	
	return float (sqrt ( x * x + y * y + z * z));
}

float D3::distance (D3 * b) const
{
	assert (is_initialized ());
	assert (b != NULL);
	assert (b->is_initialized ());

	D3 * const_this = const_cast <D3 *> (this);
	assert (const_this != NULL);
	assert (const_this->is_initialized ());
	
	D3 * diffrence =  * const_this - b;

	float result = diffrence->length ();

	delete diffrence;

	return result;
}

D3 * D3::operator- () const
{
	assert (is_initialized ());
	
	return new D3 (- x , - y, - z);
}

D3 * D3::operator* (float a) const
{
	assert (is_initialized ());
	
	return new D3 (a * x , a * y, a * z);
}

D3 * D3::operator/ (float a) const
{
	assert (is_initialized ());
	
	return new D3 (x / a,  y / a, z / a);
}

D3 * D3::operator+ (D3 * b) const
{
	assert (is_initialized ());
	assert (b != NULL);
	assert (b->is_initialized ());
	
	return new D3 (x + b->x , y + b->y, z + b->z);
}

D3 * D3::operator- (D3 * b) const
{
	assert (is_initialized ());
	assert (b != NULL);
	assert (b->is_initialized ());
	
	return new D3 (x - b->x , y - b->y, z - b->z);
}

void D3::operator+= (D3 * b)
{
	assert (is_initialized ());
	assert (b != NULL);
	assert (b->is_initialized ());
	
	x += b->x;
	y += b->y;
	z += b->z;
}

bool D3::operator== (D3 * b) const
{
	assert (is_initialized ());
	assert (b != NULL);
	assert (b->is_initialized ());
	
	return (x == b->x) && (y == b->y) && (z == b->z);
}

float D3::get_x () const
{
	assert (is_initialized ());
	
	return x;
}

float D3::get_y () const
{
	assert (is_initialized ());
	
	return y;
}

float D3::get_z () const
{
	assert (is_initialized ());
	
	return z;
}
