#ifndef RADAKAN_MOVABLE_MODEL_HPP
#define RADAKAN_MOVABLE_MODEL_HPP

#include "model.hpp"

using namespace std;

namespace OgreOde
{
	class Body;
	class Geometry;
}
namespace Radakan
{
	
	///	Movable_Model is a model that can move.
	class Movable_Model :
		public Model
	{
		public :
			Movable_Model (Reference <Items :: Item> new_item, Ogre :: Vector3 position, float scale, boost :: shared_ptr <OgreOde :: Geometry> new_geometry, boost :: shared_ptr <OgreOde :: Body> new_body);
			virtual ~Movable_Model ();
			virtual bool is_initialized () const;
			
			static string get_class_name ();
			
			//	I'll move in my front direction.
			void move (float top_speed);

			//	If no - or zero - ax specified, I'll turn around my top direction.
			void turn (float top_radian_angle_speed, Ogre :: Vector3 ax = zero_vector);

			//	Reset the orientation.
			void reset ();

		private :
			boost :: shared_ptr <OgreOde :: Body> body;
	};
}

#endif	//	RADAKAN_MOVABLE_BODY_HPP
