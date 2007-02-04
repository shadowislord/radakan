#include "representation.hpp"

using namespace std;
using namespace tsl;

//  constructor
Representation ::
	Representation (string item_name, Ogre :: SceneNode & new_node) :
	Object (item_name + "'s representation"),
	btRigidBody
	(
		1,
		new btDefaultMotionState
		(
			btTransform (btQuaternion (0, 0, 0, 1), to_btVector3 (new_node . getPosition ()))
		),
		new btSphereShape (10)
	),
//	parent_item (new_parent_item),
	node (new_node)
{
	assert (Object :: is_initialized ());

	update_scene_node ();

	assert (Representation :: is_initialized ());
}

//  destructor
Representation ::
	~Representation ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (Representation :: is_initialized ());
	
	assert (Object :: is_initialized ());
}

//	virtual
bool Representation ::
	is_initialized ()
	const
{
	return warn <Representation> (Object :: is_initialized ());
}

//	static
string Representation ::
	get_class_name ()
{
	return "Representation";
}

Ogre :: Quaternion Representation ::
	get_orientation  () const
{
	assert (Representation :: is_initialized ());

	return node . getOrientation ();
}

void Representation ::
	set_orientation  (const Ogre :: Quaternion & new_orientation)
{
	assert (Representation :: is_initialized ());

	btQuaternion q (new_orientation . x, new_orientation . y, new_orientation . z, new_orientation . w);
	get_transformation () . setRotation (q);
	
	update_scene_node ();
}

Ogre :: Vector3 Representation ::
	get_position () const
{
	assert (Representation :: is_initialized ());

	return node . getPosition ();
}

Ogre :: Vector3 Representation ::
	get_front_direction () const
{
	assert (Representation :: is_initialized ());

	return get_orientation () * Ogre :: Vector3 (0, 0, 1);
}

Ogre :: Vector3 Representation ::
	get_side_direction () const
{
	assert (Representation :: is_initialized ());

	return get_orientation () * Ogre :: Vector3 (1, 0, 0);
}

Ogre :: Vector3 Representation ::
	get_top_direction () const
{
	assert (Representation :: is_initialized ());

	return get_orientation () * Ogre :: Vector3 (0, 1, 0);
}

void Representation ::
	set_scale (float scale)
{
	assert (Representation :: is_initialized ());

	//	TODO set the scale for bullet to.

	node . setScale (scale, scale, scale);
}

void Representation ::
	set_position (float x, float y, float z)
{
	assert (Representation :: is_initialized ());

	get_transformation () . setOrigin (btVector3 (x, y, z));
	
//	This somehow doesn't do what it's supposed to do. (It doesn't change anything.)
//	translate (new_position);

//	Keep the Ogre :: SceneNode in sync.
	update_scene_node ();
}

void Representation ::
	update_scene_node ()
{
	assert (Representation :: is_initialized ());

	btVector3 origin = get_transformation () . getOrigin ();
	btQuaternion rot = get_transformation () . getRotation ();
	node . setPosition (origin . x (), origin . y (), origin . z ());
	node . setOrientation (rot . getW (), rot . x (), rot . y (), rot . z ());
}

//	private
btTransform & Representation ::
	get_transformation () const
{
	assert (Representation :: is_initialized ());

	return dynamic_cast <btDefaultMotionState *> (const_cast <Representation *> (this) -> getMotionState ()) -> m_graphicsWorldTrans;
}

void Representation ::
	move (float distance)
{
	assert (Representation :: is_initialized ());

	Ogre :: Vector3 w = get_position () - distance * get_front_direction ();
	set_position (w . x, w . y, w . z);
		
	update_scene_node ();
}

void Representation ::
	turn (float radian_angle, Ogre :: Vector3 ax)
{
	debug () << "turn (" << radian_angle << ", (" << ax . x << ", " << ax . y << ", " << ax . z << "))" << endl;
	assert (Representation :: is_initialized ());

	set_orientation (get_orientation () * Ogre :: Quaternion (Ogre :: Radian (radian_angle), ax));
}

Ogre :: Entity & Representation ::
	get_entity () const
{
	assert (Representation :: is_initialized ());

	return * dynamic_cast <Ogre :: Entity *> (node . getAttachedObject (0));
}

btVector3 & tsl :: to_btVector3 (Ogre :: Vector3 old)
{
	return * (new btVector3 (old . x, old . y, old . z));
}

Ogre :: Quaternion tsl :: make_quaternion (float radian_angle, Ogre :: Vector3 ax)
{
	return Ogre :: Quaternion (Ogre :: Radian (radian_angle), ax);
}
