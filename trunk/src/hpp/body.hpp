#ifndef RADAKAN_BODY_HPP
#define RADAKAN_BODY_HPP

#include "location.hpp"
#include "resident.hpp"

using namespace std;

namespace Radakan
{
	
	namespace Items
	{
		class Item;
	}
	
	class Body :
		public virtual Object
	{
		public :
			static string get_class_name ();
			static Reference <Body> create
			(
				float mass,
				Mathematics :: Vector_3D size,
				Mathematics :: Vector_3D position,
				Mathematics :: Quaternion orientation
			);
			
		protected :
			///	This is an abstact base class.
			Body ();
			
		public :
			virtual ~Body ();
			virtual bool is_initialized () const;
			
			virtual Mathematics :: Vector_3D get_position () const = 0;
			virtual Mathematics :: Quaternion get_orientation () const = 0;
			
			Mathematics :: Vector_3D get_front_direction () const;
			Mathematics :: Vector_3D get_side_direction () const;
			Mathematics :: Vector_3D get_top_direction () const;

			virtual Mathematics :: Vector_3D get_linear_velocity () const = 0;
			virtual Mathematics :: Vector_3D get_angular_velocity () const = 0;

			void walk (float relative_destination_linear_velocity);

			///	If no - or zero - axis specified, I'll turn around my top direction.
			void turn
			(
				float relative_destination_angular_velocity,
				Mathematics :: Vector_3D axis = Mathematics :: Vector_3D :: zero_vector
			);

			void jump ();
			
			virtual void apply_force (Mathematics :: Vector_3D force) = 0;
			virtual void apply_torque (Mathematics :: Vector_3D torque) = 0;
			
			///	Reset the position and orientation.
			virtual void reset () = 0;
	};
}

#endif	//	RADAKAN_BODY_HPP
