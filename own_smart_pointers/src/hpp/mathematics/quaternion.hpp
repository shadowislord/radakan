#ifndef MATHEMATICS_QUATERNION_HPP
#define MATHEMATICS_QUATERNION_HPP

#include "base.hpp"

#include <OgreVector3.h>
#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	#include <LinearMath/btQuaternion.h>
#endif

using namespace std;

namespace Radakan
{
	namespace Mathematics
	{
		class Vector_3D;
		
		class Quaternion :
			public Ogre :: Quaternion
		{
			public :
				static const Quaternion identity;	//	(0, 0, 0, 1)
				
				Quaternion ();
				Quaternion
				(
					const float & new_x,
					const float & new_y,
					const float & new_z,
					const float & new_w
				);
				Quaternion (const float & radian_angle, const Mathematics :: Vector_3D & axis);
				Quaternion (const string & value);
#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
				Quaternion (const btQuaternion & other);
#endif
				Quaternion (const Ogre :: Quaternion & other);
				~Quaternion ();

				string to_string () const;
#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
				btQuaternion to_bullet () const;
#endif
		};
	}
}

#endif	//	MATHEMATICS_QUATERNION_HPP
