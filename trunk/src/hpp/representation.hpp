#ifndef TSL_REPRESENTATION_HPP
#define TSL_REPRESENTATION_HPP

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include <OgreOde_Core.h>

#include "object.hpp"

using namespace std;

namespace tsl
{
	class Item;

	///	Representation is the 3D representation of an Item.
	class Representation :
		public virtual Object,
		public OgreOde :: Body
	{
		public :
			Representation (Item & new_item, OgreOde :: World & world);
			virtual ~Representation ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			Ogre :: Vector3 get_front_direction () const;
			Ogre :: Vector3 get_side_direction () const;
			Ogre :: Vector3 get_top_direction () const;
			Ogre :: Entity & get_entity () const;

			void set_scale (float scale);
			
			void move (float distance);

			///	turn horizontaly
			void turn (float radian_angle, Ogre :: Vector3 ax = Ogre :: Vector3 (0, 1, 0));

			//	We need this for 2D -> 3D convertion.
			Item & item;
				
		private :
			//	Copies are not allowed.
			Representation (const Representation & representation);

			Ogre :: SceneNode * node;
			OgreOde :: Geometry * geometry;
	};

	Ogre :: Quaternion make_quaternion (float radian_angle, Ogre :: Vector3 ax);
}

#endif	//	TSL_REPRESENTATION_HPP
