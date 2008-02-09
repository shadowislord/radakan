#include "engines/log.hpp"
#include "engines/render_engine.hpp"
#include "items/static_item.hpp"
#include "model.hpp"
#include "slot.hpp"
#include "world.hpp"

#include <OgreEntity.h>
#include <OgreSceneManager.h>

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	#include <OgreOdeGeometry.h>
	#include <OgreOdeSpace.h>
	#include <OgreOdeBody.h>
	#include <OgreOdeWorld.h>
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	#include <btBulletDynamicsCommon.h>
	#include <btBulletDynamicsCommon.h>
#else
#endif

using namespace std;
using namespace Radakan;

//	static
string Model ::
	get_class_name ()
{
	return "Model";
}

//  constructor
Model ::
	Model (Reference <Items :: Item> new_item, Mathematics :: Vector_3D position) :
	Object (new_item . get_name () + "'s model"),
	Location <Items :: Item> (1),	//	A Model corresponds to a single Item.
	item (new_item),
	node (World :: get () -> root_node -> createChildSceneNode (name)),
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	geometry (create_geometry (item)),
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	motion_state (new btDefaultMotionState),
	collision_shape (create_collsion_shape (item)),
	body (new btRigidBody (btRigidBody :: btRigidBodyConstructionInfo
		(item -> mass, motion_state . get (), collision_shape . get ()))),
#else
#endif
	entity
	(
		Engines :: Render_Engine :: get () -> get_scene_manager () -> createEntity
		(
			name + "'s entity", item -> mesh_data -> file_name
		)
	)
{
	Engines :: Log :: trace (me, Model :: get_class_name (), "", new_item . get_name (),
		position . to_string ());
	assert (item . points_to_object ());
	assert (item -> is_initialized ());

	add_automatic_destruction_prevention ("construction of " + get_class_name ());
	add (item);
	remove_automatic_destruction_prevention ("construction of " + get_class_name ());
	seal ();

//	World :: get () . root_node . addChild (this);

	node -> setPosition (position);
	node -> setScale (item -> mesh_data -> scale, item -> mesh_data -> scale,
		item -> mesh_data -> scale);
	node -> attachObject (entity . get ());

	assert (node -> numAttachedObjects () == 1);
	
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	entity -> setUserObject (geometry . get ());
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
#else
#endif

	if (item . is_castable <Items :: Static_Item> ())
	{
		Engines :: Log :: log (me) << "I'm a static model." << endl;

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
		geometry -> setUserObject (entity . get ());
		geometry -> setPosition (position);
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
#else
#endif
	}

	add_automatic_destruction_prevention ("construction of " + get_class_name ());
	{
		item -> set_model (Reference <Model> (this));
	}
	remove_automatic_destruction_prevention ("construction of " + get_class_name ());

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	if (! item -> mesh_data -> solid)
	{
		geometry -> disable ();
	}
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
#else
#endif

	if (! item -> mesh_data -> material_file_name . empty ())
	{
		entity -> setMaterialName (item -> mesh_data -> material_file_name);
	}

	node -> setOrientation (item -> mesh_data -> default_orientation);

	assert (Model :: is_initialized ());
}

//  destructor
Model ::
	~Model ()
{
	Engines :: Log :: trace (me, Model :: get_class_name (), "~");
	assert (Model :: is_initialized ());

	prepare_for_destruction ();

	//	There's no need to remove myself from my item.

	assert (node -> numAttachedObjects () == 1);

	Engines :: Render_Engine :: get () -> get_scene_manager ()
		-> destroyMovableObject (entity . get ());

	assert (node -> numAttachedObjects () == 0);

	node -> getParent () -> removeChild (node -> getName ());
}

//	virtual
bool Model ::
	is_initialized ()
	const
{
	assert (this -> Location <Items :: Item> :: is_initialized ());
	assert (is_sealed ());
	assert (node -> getParent () != NULL);
	assert (World :: get () -> root_node . get () != NULL);
	
	assert (node -> getParent () == World :: get () -> root_node . get ());
	assert (node -> numAttachedObjects () <= 2);

	return true;
}

Mathematics :: Vector_3D Model ::
	get_position () const
{
	assert (Model :: is_initialized ());

	return node -> getPosition ();
}

Mathematics :: Vector_3D Model ::
	get_front_direction () const
{
	assert (Model :: is_initialized ());

	return node -> getOrientation () * Mathematics :: Vector_3D :: z_axis;
}

Mathematics :: Vector_3D Model ::
	get_side_direction () const
{
	assert (Model :: is_initialized ());

	return node -> getOrientation () * Mathematics :: Vector_3D :: x_axis;
}

Mathematics :: Vector_3D Model ::
	get_top_direction () const
{
	assert (Model :: is_initialized ());

	return node -> getOrientation () * Mathematics :: Vector_3D :: y_axis;
}

Ogre :: Quaternion Model ::
	get_orientation () const
{
	assert (Model :: is_initialized ());

	return node -> getOrientation ();
}

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
void Model ::
	set_space (boost :: shared_ptr <OgreOde :: Space> new_space)
{
	/*boost :: shared_ptr <OgreOde :: Space> old_space (geometry -> getSpace ());
	if (old_space)
	{
		old_space -> removeGeometry (* geometry . get ());
	}*/
	new_space -> addGeometry (* geometry . get ());
}
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
void Model ::
	sync ()
{
	btTransform transformation;

	motion_state -> getWorldTransform (transformation);

	btQuaternion orientation (transformation . getRotation ());
	node -> setOrientation
		(orientation . x (), orientation . y (), orientation . z (), orientation . w ());
	node -> setPosition (Mathematics :: Vector_3D (transformation . getOrigin ()));
}
#else
#endif

Ogre :: Quaternion Radakan :: make_quaternion
	(float radian_angle, Mathematics :: Vector_3D axis)
{
	return Ogre :: Quaternion (Ogre :: Radian (radian_angle), axis);
}

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
boost :: shared_ptr <OgreOde :: Geometry> Radakan ::
	create_geometry (Reference <Items :: Item> item)
{
	assert (item . points_to_object ());
	assert (item -> is_initialized ());
	assert (! item -> has_model ());

	if (item -> size . y <= 0.01)
	{
		return boost :: shared_ptr <OgreOde :: Geometry>
		(
			new OgreOde :: InfinitePlaneGeometry
			(
				Ogre :: Plane (Mathematics :: Vector_3D :: y_axis, 0),
				World :: get () -> ogre_ode_world . get ()
			)
		);
	}
	else
	{
		return boost :: shared_ptr <OgreOde :: Geometry>
		(
			new OgreOde :: BoxGeometry
				(item -> size, World :: get () -> ogre_ode_world . get ())
		);
	}
}
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
boost :: shared_ptr <btCollisionShape> Radakan ::
	create_collsion_shape (Reference <Items :: Item> item)
{
	assert (item . points_to_object ());
	assert (item -> is_initialized ());
	assert (! item -> has_model ());

	if (item -> size . y <= 0.01)
	{
		return boost :: shared_ptr <btCollisionShape>
			(new btStaticPlaneShape (btVector3 (0, 1, 0), 0));
	}
	else
	{
		return boost :: shared_ptr <btCollisionShape>
			(new btBoxShape (btVector3 (1, 1, 1)));
	}
}
#else
#endif
