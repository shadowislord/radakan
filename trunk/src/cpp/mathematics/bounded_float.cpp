#include "mathematics/bounded_float.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Mathematics;

const float Radakan :: Mathematics :: infinite = 1e200 * 1e200;

Bounded_Float ::
	Bounded_Float (float new_val, float new_min, float new_max) :
	minimum (new_min),
	maximum (new_max)
{
	assert (new_min <= new_max);
	
	set_val (new_val);
}

Bounded_Float :: 
	~Bounded_Float ()
{

}

float Bounded_Float ::
	get_val ()
	const
{
	return val;
}

void Bounded_Float :: set_val (float new_val)
{
	assert (minimum <= new_val);
	assert (new_val <= maximum);

	val = new_val;
}

//	+=
Bounded_Float Bounded_Float ::
	operator+= (float operand)
{
	set_val (val + operand);
	
	return * this;
}

//	=
Bounded_Float Bounded_Float ::
	operator= (const Bounded_Float & operand)
{
	set_val (operand . get_val ());
	return (* this);
}
Bounded_Float Bounded_Float ::
	operator= (float operand)
{
	set_val (operand);
	return (* this);
}

//	+
float Bounded_Float ::
	operator+ (const Bounded_Float & operand)
	const
{
	return get_val () + operand . get_val ();
}
float Bounded_Float ::
	operator+ (float operand)
	const
{
	return get_val () + operand;
}

//	-
float Bounded_Float ::
	operator- (const Bounded_Float & operand)
	const
{
	return get_val () - operand . get_val ();
}
float Bounded_Float ::
	operator- (float operand)
	const
{
	return get_val () - operand;
}

//	*
float Bounded_Float ::
	operator* (const Bounded_Float & operand)
	const
{
	return get_val () * operand . get_val ();
}
float Bounded_Float ::
	operator* (float operand)
	const
{
	return get_val () * operand;
}

//	/
float Bounded_Float ::
	operator/ (const Bounded_Float & operand)
	const
{
	return get_val () / operand . get_val ();
}
float Bounded_Float ::
	operator/ (float operand)
	const
{
	return get_val () / operand;
}
