#include "engines/log.hpp"
#include "mathematics/quaternion.hpp"
#include "mathematics/vector_3d.hpp"

#include <sstream>

using namespace std;
using namespace Radakan;
using namespace Radakan :: Mathematics;

//	static
const Quaternion Quaternion ::
	identity (1, 0, 0, 0);

Quaternion ::
	Quaternion () :
	Ogre :: Quaternion (identity)
{

}

Quaternion ::
	Quaternion
	(
		const float & new_w,
		const float & new_x,
		const float & new_y,
		const float & new_z
	) :
	Ogre :: Quaternion (new_w, new_x, new_y, new_z)
{

}

Quaternion ::
	Quaternion (const float & radian_angle, const Mathematics :: Vector_3D & axis) :
	Ogre :: Quaternion (Ogre :: Radian (radian_angle), axis)
{

}

Quaternion ::
	Quaternion (const string & value) :
	Ogre :: Quaternion (identity)
{
	istringstream stream;
	stream . str (value);
	stream . exceptions (istringstream :: failbit);

	float temp;
	char trash;

	for (int i = 0; i < 4; i ++)	//	x, y, z & w
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
				w = temp;
				break;
			case (1) :
				x = temp;
				break;
			case (2) :
				y = temp;
				break;
			case (3) :
				z = temp;
				break;
		}
	}
}

#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
Quaternion ::
	Quaternion (const btQuaternion & other) :
	Ogre :: Quaternion (other . w (), other . x (), other . y (), other . z ())
{

}
#endif

Quaternion ::
	Quaternion (const Ogre :: Quaternion & other) :
	Ogre :: Quaternion (other)
{

}

Quaternion ::
	~Quaternion ()
{

}

string Quaternion ::
	to_string ()
	const
{
	return "(" + Radakan :: to_string (w) + ", " + Radakan :: to_string (x) + ", "
		+ Radakan :: to_string (y)+ ", " + Radakan :: to_string (z) + ")";
}

#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
btQuaternion Quaternion ::
	to_bullet ()
	const
{
	return btQuaternion (x, y, z, w);
}
#endif
