#ifndef TSL_BODY_HPP
#define TSL_BODY_HPP

#include "set.hpp"
#include "environment.hpp"
#include "item.hpp"

using namespace std;

namespace TSL
{
	const Ogre :: Vector3 x_axis (1, 0, 0);
	const Ogre :: Vector3 y_axis (0, 1, 0);	//	upwards
	const Ogre :: Vector3 z_axis (0, 0, 1);

	///	Body is the 3D representation of an Item.
	class Body :
		public Set <Item>
	{
		public :
			Body (Item & new_item, Ogre :: Vector3 position, float scale, OgreOde :: Geometry & new_geometry);
			virtual ~Body ();
			virtual bool is_initialized () const;
			
			static const string class_name;
			
			Ogre :: Vector3 get_front_direction () const;
			Ogre :: Vector3 get_side_direction () const;
			Ogre :: Vector3 get_top_direction () const;

			void set_scale (float scale);
			void set_material (string name);

			void set_space (OgreOde :: Space & new_space);

			Item & item;
			Ogre :: SceneNode & node;

		protected :
			OgreOde :: Geometry & geometry;
	};

	Ogre :: Quaternion make_quaternion (float radian_angle, Ogre :: Vector3 ax);
}

#endif	//	TSL_BODY_HPP
