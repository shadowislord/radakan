#ifndef RADAKAN_MODEL_HPP
#define RADAKAN_MODEL_HPP

#include "location.hpp"
#include "resident.hpp"

using namespace std;

namespace Ogre
{
	class SceneNode;
}

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	namespace OgreOde
	{
		class Geometry;
		class Space;
	}
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	class btMotionState;
	class btCollisionShape;
	class btRigidBody;
#else
#endif

namespace Radakan
{
	namespace Items
	{
		class Item;
	}

	///	Model is a 3D representation of an Items :: Item.
	class Model :
		public Resident <Model>,
		public Location <Items :: Item>
	{
		public :
			Model (Reference <Items :: Item> new_item, Mathematics :: Vector_3D position);
			virtual ~Model ();
			virtual bool is_initialized () const;
			
			static string get_class_name ();
			
			Mathematics :: Vector_3D get_position () const;
			
			Mathematics :: Vector_3D get_front_direction () const;
			Mathematics :: Vector_3D get_side_direction () const;
			Mathematics :: Vector_3D get_top_direction () const;

			Mathematics :: Quaternion get_orientation () const;

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
			void set_space (boost :: shared_ptr <OgreOde :: Space> new_space);
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
			void sync ();
#else
#endif
			
			Reference <Items :: Item> item;
			
		protected :
			boost :: shared_ptr <Ogre :: SceneNode> node;

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
			boost :: shared_ptr <OgreOde :: Geometry> geometry;
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
			boost :: scoped_ptr <btMotionState> motion_state;
			boost :: shared_ptr <btCollisionShape> collision_shape;
			boost :: scoped_ptr <btRigidBody> body;
#else
#endif
			boost :: scoped_ptr <Ogre :: Entity> entity;
	};

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	boost :: shared_ptr <OgreOde :: Geometry> create_geometry (Reference <Items :: Item> item);
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	boost :: shared_ptr <btCollisionShape> create_collsion_shape
		(Reference <Items :: Item> item);
#else
#endif
}

#endif	//	RADAKAN_MODEL_HPP
