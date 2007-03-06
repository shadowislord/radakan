#ifndef TSL_BODY_HPP
#define TSL_BODY_HPP

#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include <OgreOde_Core.h>

#include "observable.hpp"

using namespace std;

namespace tsl
{
	const Ogre :: Vector3 zero (0, 0, 0);
	const Ogre :: Vector3 x_axis (1, 0, 0);
	const Ogre :: Vector3 y_axis (0, 1, 0);	//	upwards
	const Ogre :: Vector3 z_axis (0, 0, 1);

	class Item;

	///	Body is the 3D representation of an Item.
	class Body :
		public Observable <Body>,
		public Ogre :: SceneNode
	{
		public :
   Body
			(
				Ogre :: SceneManager & scene_manager,
				Observable <Body> & observer,
				Ogre :: Entity & new_entity,
				OgreOde :: Geometry & new_geometry
			);
			virtual ~Body ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			bool is_mobile () const;

			Ogre :: Vector3 get_front_direction () const;
			Ogre :: Vector3 get_side_direction () const;
			Ogre :: Vector3 get_top_direction () const;

			float get_scale ();
			void set_scale (float scale);
			void set_material (string name);

			//	If no - or zero - ax specified, I'll move in my front direction.
			void move (float distance, Ogre :: Vector3 ax = zero);

			//	If no - or zero - ax specified, I'll turn around my top direction.
			void turn (float radian_angle, Ogre :: Vector3 ax = zero);

		private :
			//	Copies are not allowed.
			Body (const Body & body);

			Ogre :: Entity & entity;
			OgreOde :: Geometry & geometry;
			OgreOde :: Body * body;

			const Ogre :: Node * root_node;
	};

	Ogre :: Quaternion make_quaternion (float radian_angle, Ogre :: Vector3 ax);
}

#endif	//	TSL_BODY_HPP
