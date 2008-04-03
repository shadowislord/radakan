#ifndef MATHEMATICS_VECTOR_3D_HPP
#define MATHEMATICS_VECTOR_3D_HPP

#include "base.hpp"

#include <OgreVector3.h>
#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	#include <LinearMath/btVector3.h>
#endif

using namespace std;

namespace Radakan
{
	namespace Mathematics
	{
		class Vector_3D :
			public Ogre :: Vector3
		{
			public :
				static const Vector_3D zero_vector;	//	(0, 0, 0)
				static const Vector_3D x_axis;	//	(1, 0, 0)
				static const Vector_3D y_axis;	//	(0, 1, 0) - upwards
				static const Vector_3D z_axis;	//	(0, 0, 1)
				
				Vector_3D ();
				Vector_3D (const float & new_x, const float & new_y, const float & new_z);
				Vector_3D (const string & value);
#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
				Vector_3D (const btVector3 & other);
#endif
				Vector_3D (const Ogre :: Vector3 & other);
				~Vector_3D ();

				string to_string () const;
#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
				btVector3 to_bullet () const;
#endif
				///	'<' doesn't act as expected (i.e. a 'total order') when inherited.
				bool operator< (const Vector_3D & other) const;
		};
	}
}

#endif	//	MATHEMATICS_VECTOR_3D_HPP
