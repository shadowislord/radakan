#ifndef TSL_REPRESENTATION_HPP
#define TSL_REPRESENTATION_HPP

#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>

#include "object.hpp"

using namespace std;

namespace tsl
{
	class Item;

	///	Representation is the 3D representation of an Item.

	class Representation :
		public virtual Object,
		public btRigidBody
	{
		public :
			Representation (Item & new_item, Ogre :: SceneNode & new_node);
			virtual ~Representation ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			Ogre :: Quaternion get_orientation () const;
			void set_orientation (const Ogre :: Quaternion & new_orientation);
			Ogre :: Vector3 get_position () const;
			void set_position (float x, float y, float z);
			void set_scale (float scale);
			void update_scene_node ();
			Ogre :: Entity & get_entity () const;

			void move (float distance);

			///	turn horizontaly
			void turn (float radian_angle);

			Item & parent_item;
				
		private :
			//	Copies are not allowed.
			Representation (const Representation & representation);
			
			btTransform & get_transformation () const;

			Ogre :: SceneNode & node;
	};

	btVector3 & to_btVector3 (Ogre :: Vector3 old);
}

#endif	//	TSL_REPRESENTATION_HPP
