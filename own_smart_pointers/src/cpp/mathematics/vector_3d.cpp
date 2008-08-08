#include "engines/log.hpp"
#include "mathematics/vector_3d.hpp"

#include <sstream>

using namespace std;
using namespace Radakan;
using namespace Radakan :: Mathematics;

//	static
const Vector_3D Vector_3D ::
	zero_vector (0, 0, 0);

//	static
const Vector_3D Vector_3D ::
	x_axis (1, 0, 0);

//	static
const Vector_3D Vector_3D ::
	y_axis (0, 1, 0);

//	static
const Vector_3D Vector_3D ::
	z_axis (0, 0, 1);

Vector_3D ::
	Vector_3D () :
	Ogre :: Vector3 (0, 0, 0)
{

}

Vector_3D ::
	Vector_3D (const float & new_x, const float & new_y, const float & new_z) :
	Ogre :: Vector3 (new_x, new_y, new_z)
{

}

Vector_3D ::
	Vector_3D (const string & value) :
	Ogre :: Vector3 (0, 0, 0)
{
	istringstream stream;
	stream . str (value);
	stream . exceptions (istringstream :: failbit);

	float temp;
	char trash;

	for (int i = 0; i < 3; i ++)	//	x, y & z
	{
		for (int j = 0; j < 2; j ++)	//	maximally two unusalbe characters
		{
			try
			{
				stream >> temp;
				break;
			}
			catch (exception & e)
			{
				stream . clear ();
				stream >> trash;
			}
		}
		
		switch (i)
		{
			case (0) :
				x = temp;
				break;
			case (1) :
				y = temp;
				break;
			case (2) :
				z = temp;
				break;
		}
	}
}

#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
Vector_3D ::
	Vector_3D (const btVector3 & other) :
	Ogre :: Vector3 (other . x (), other . y (), other . z ())
{

}
#endif

Vector_3D ::
	Vector_3D (const Ogre :: Vector3 & other) :
	Ogre :: Vector3 (other)
{

}

Vector_3D ::
	~Vector_3D ()
{

}

string Vector_3D ::
	to_string ()
	const
{
	return "(" + Radakan :: to_string (x) + ", " + Radakan :: to_string (y) + ", "
		+ Radakan :: to_string (z) + ")";
}

#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
btVector3 Vector_3D ::
	to_bullet ()
	const
{
	return btVector3 (x, y, z);
}
#endif

bool Vector_3D ::
	operator< (const Vector_3D & other)
	const
{
	return (x < other . x) || ((x == other . x) && (y < other . y))
		|| ((x == other . x) && (y == other . y) && (z < other . z));
}
