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
			
			btQuaternion get_rotation () const;
			void set_rotation (const btQuaternion & new_rotation);
			btVector3 get_position () const;
			void set_position (const btVector3 & new_position);
			void update_scene_node ();

			void move (float distance);

			///	turn horizontaly
			void turn (float radian_angle);


			Item & parent_item;
			Ogre :: SceneNode & node;
				
		private :
			//	Copies are not allowed.
			Representation (const Representation & representation);
			
			btDefaultMotionState * get_motion_state () const;
	};

	btVector3 & to_btVector3 (Ogre :: Vector3 old);
}

#endif	//	TSL_REPRESENTATION_HPP
