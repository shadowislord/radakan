#include "mathematics/bounded_float.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Mathematics;

const float Radakan :: Mathematics :: infinite = numeric_limits <float> :: infinity ();

Bounded_Float ::
	Bounded_Float (float new_value, float new_minimum, float new_maximum) :
	minimum (new_minimum),
	maximum (new_maximum)
{
	assert (new_minimum <= new_maximum);
	
	set_value (new_value);
}

Bounded_Float :: 
	~Bounded_Float ()
{

}

float Bounded_Float ::
	get_value ()
	const
{
	return value;
}

void Bounded_Float :: set_value (float new_value)
{
	assert (minimum <= new_value);
	assert (new_value <= maximum);

	value = new_value;
}

//	=
Bounded_Float Bounded_Float ::
	operator= (const Bounded_Float & operand)
{
	set_value (operand . get_value ());
	return (* this);
}
Bounded_Float Bounded_Float ::
	operator= (float operand)
{
	set_value (operand);
	return (* this);
}

//	+=
Bounded_Float Bounded_Float ::
	operator+= (float operand)
{
	set_value (value + operand);
	
	return * this;
}

//	*=
Bounded_Float Bounded_Float ::
	operator*= (float operand)
{
	set_value (value * operand);
	
	return * this;
}

//	>
bool Bounded_Float ::
	operator> (const Bounded_Float & operand)
	const
{
	return value > operand . get_value ();
}
bool Bounded_Float ::
	operator> (float operand)
	const
{
	return value > operand;
}

//	>=
bool Bounded_Float ::
	operator>= (const Bounded_Float & operand)
	const
{
	return value >= operand . get_value ();
}
bool Bounded_Float ::
	operator>= (float operand)
	const
{
	return value >= operand;
}

//	==
bool Bounded_Float ::
	operator== (const Bounded_Float & operand)
	const
{
	return value == operand . get_value ();
}
bool Bounded_Float ::
	operator== (float operand)
	const
{
	return value == operand;
}

//	<=
bool Bounded_Float ::
	operator<= (const Bounded_Float & operand)
	const
{
	return value <= operand . get_value ();
}
bool Bounded_Float ::
	operator<= (float operand)
	const
{
	return value <= operand;
}

//	<
bool Bounded_Float ::
	operator< (const Bounded_Float & operand)
	const
{
	return value < operand . get_value ();
}
bool Bounded_Float ::
	operator< (float operand)
	const
{
	return value < operand;
}

//	+
float Bounded_Float ::
	operator+ (const Bounded_Float & operand)
	const
{
	return value + operand . get_value ();
}
float Bounded_Float ::
	operator+ (float operand)
	const
{
	return value + operand;
}

//	-
float Bounded_Float ::
	operator- (const Bounded_Float & operand)
	const
{
	return value - operand . get_value ();
}
float Bounded_Float ::
	operator- (float operand)
	const
{
	return value - operand;
}

//	*
float Bounded_Float ::
	operator* (const Bounded_Float & operand)
	const
{
	return value * operand . get_value ();
}
float Bounded_Float ::
	operator* (float operand)
	const
{
	return value * operand;
}

//	/
float Bounded_Float ::
	operator/ (const Bounded_Float & operand)
	const
{
	return value / operand . get_value ();
}
float Bounded_Float ::
	operator/ (float operand)
	const
{
	return value / operand;
}
