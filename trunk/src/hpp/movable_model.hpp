#ifndef TSL_MOVABLE_MODEL_HPP
#define TSL_MOVABLE_MODEL_HPP

#include "model.hpp"

using namespace std;

namespace TSL
{
	
	///	Movable_Model is a model that can move.
	class Movable_Model :
		public Model
	{
		public :
			Movable_Model (Items :: Item & new_item, Ogre :: Vector3 position, float scale, OgreOde :: Geometry & new_geometry, OgreOde :: Body & new_body);
			virtual ~Movable_Model ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();
			
			//	I'll move in my front direction.
			void move (float top_speed);

			//	If no - or zero - ax specified, I'll turn around my top direction.
			void turn (float top_radian_angle_speed, Ogre :: Vector3 ax = zero_vector);

			//	Reset the orientation.
			void reset ();

		private :
			OgreOde :: Body & body;
	};
}

#endif	//	TSL_MOVABLE_BODY_HPP
