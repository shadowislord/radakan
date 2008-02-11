#ifndef RADAKAN_MOVABLE_MODEL_HPP
#define RADAKAN_MOVABLE_MODEL_HPP

#include "model.hpp"

using namespace std;

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	namespace OgreOde
	{
		class Body;
		class Geometry;
	}
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
#else
#endif

namespace Radakan
{
	
	///	Movable_Model is a model that can move.
	class Movable_Model :
		public Model
	{
		public :
			Movable_Model
				(Reference <Items :: Item> new_item, Mathematics :: Vector_3D position);
			virtual ~Movable_Model ();
			virtual bool is_initialized () const;
			
			static string get_class_name ();
			
			///	I'll move in my front direction.
			///	
			void move (float relative_destination_linear_velocity);

			///	If no - or zero - axis specified, I'll turn around my top direction.
			void turn
			(
				float relative_destination_angular_velocity,
				Mathematics :: Vector_3D axis = Mathematics :: Vector_3D :: zero_vector
			);

			///	Reset the position and orientation.
			void reset ();

		private :
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
			boost :: shared_ptr <OgreOde :: Body> body;
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
#else
#endif
	};
}

#endif	//	RADAKAN_MOVABLE_BODY_HPP
