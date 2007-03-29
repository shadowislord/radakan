#ifndef TSL_BODY_HPP
#define TSL_BODY_HPP

#include "disjoint_set.hpp"
#include "environment.hpp"
#include "item.hpp"

using namespace std;

namespace tsl
{
	const Ogre :: Vector3 x_axis (1, 0, 0);
	const Ogre :: Vector3 y_axis (0, 1, 0);	//	upwards
	const Ogre :: Vector3 z_axis (0, 0, 1);

	///	Body is the 3D representation of an Item.
	class Body :
		public Disjoint_Set <Item>
	{
		public :
			Body (Item & new_item, Ogre :: Vector3 position, float scale);
			virtual ~Body ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			///	'add' always returns 'false'.
			virtual bool add (Item & item);
			
			///	'move' always returns 'false'.
			virtual bool move (Item & item, Disjoint_Set <Item> & destination);

			bool is_mobile () const;

			Ogre :: Vector3 get_front_direction () const;
			Ogre :: Vector3 get_side_direction () const;
			Ogre :: Vector3 get_top_direction () const;

			void set_scale (float scale);
			void set_material (string name);

			//	I'll move in my front direction.
			void move (float top_speed, float turn_length);

			//	If no - or zero - ax specified, I'll turn around my top direction.
			void turn (float top_radian_angle_speed, float turn_length, Ogre :: Vector3 ax = zero_vector);

			//	Reset the orientation.
			void reset ();

			Item & item;
			Ogre :: SceneNode & node;
			OgreOde :: Geometry & geometry;

		private :
			//	Copies are not allowed.
			Body (const Body & body);

			Ogre :: Entity & entity;

			///	only for movable items
			OgreOde :: Body * body;
	};

	Ogre :: Quaternion make_quaternion (float radian_angle, Ogre :: Vector3 ax);
}

#endif	//	TSL_BODY_HPP
