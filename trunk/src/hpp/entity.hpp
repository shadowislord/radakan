#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "object.hpp"
#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>

using namespace std;

namespace tsl
{

	///	Entity is the base class for all in-game entity classes.

	class Entity :
		public virtual Object,
		public btRigidBody
	{
		public :
			Entity
				(bool new_movable,
				bool new_solid,
				bool new_visible,
				float new_volume,
				float new_weight,
				btVector3 new_position,
				Ogre :: SceneNode & new_node);
			virtual ~Entity ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			virtual float get_total_weight () const;

			btQuaternion get_rotation () const;
			void set_rotation (const btQuaternion & new_rotation);
			btVector3 get_position () const;
			void set_position (const btVector3 & new_position);
			void update_scene_node ();
		
			const bool movable;		//	false means that the object is static
			const bool solid;		//	false means that you can pass through it
			const float volume;		//	in litres
			const float weight;		//	in kilograms
			Ogre :: SceneNode * node;	//	create through Scene_Manager

		private :
			//	Copies are not allowed.
			Entity (const Entity & entity);
			
			btDefaultMotionState * get_motion_state () const;
	};

	string get_name (Ogre :: SceneNode & node);
}

#endif
