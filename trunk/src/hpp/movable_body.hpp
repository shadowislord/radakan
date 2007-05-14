#ifndef TSL_MOVABLE_BODY_HPP
#define TSL_MOVABLE_BODY_HPP

#include "body.hpp"

using namespace std;

namespace TSL
{
	
	class Movable_Body :
		public Body
	{
		public :
			Movable_Body (Item & new_item, Ogre :: Vector3 position, float scale, OgreOde :: Geometry & new_geometry, OgreOde :: Body & new_body);
			virtual ~Movable_Body ();
			virtual bool is_initialized () const;
			
			static const string class_name;
			
			//	I'll move in my front direction.
			void move (float top_speed, float turn_length);

			//	If no - or zero - ax specified, I'll turn around my top direction.
			void turn (float top_radian_angle_speed, float turn_length, Ogre :: Vector3 ax = zero_vector);

			//	Reset the orientation.
			void reset ();

		private :
			OgreOde :: Body & body;
	};
}

#endif	//	TSL_MOVABLE_BODY_HPP
