#include "representation.hpp"

using namespace std;
using namespace tsl;

//  constructor
Representation ::
	Representation (Item & new_item, Ogre :: SceneNode & new_node) :
	Object ("anonymous representation"),
	btRigidBody
	(
		1,
		new btDefaultMotionState
		(
			btTransform (btQuaternion (0, 0, 0, 1), to_btVector3 (new_node . getPosition ()))
		),
		new btSphereShape (10)
	),
	parent_item (new_item),
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

btQuaternion Representation ::
	get_rotation  () const
{
	assert (Representation :: is_initialized ());

	return get_motion_state () -> m_graphicsWorldTrans . getRotation ();
}

void Representation ::
	set_rotation  (const btQuaternion & new_rotation)
{
	assert (Representation :: is_initialized ());

	get_motion_state () -> m_graphicsWorldTrans . setRotation (new_rotation);
	update_scene_node ();
}

btVector3 Representation ::
	get_position () const
{
	assert (Representation :: is_initialized ());

	return get_motion_state () -> m_graphicsWorldTrans . getOrigin ();
}

void Representation ::
	set_position (const btVector3 & new_position)
{
	assert (Representation :: is_initialized ());

	get_motion_state () -> m_graphicsWorldTrans . setOrigin (new_position);
	
//	This somehow doesn't do what it's supposed to do. (It doesn't change anything.)
//	translate (new_position);

//	Keep the Ogre :: SceneNode in sync.
	update_scene_node ();
}

void Representation ::
	update_scene_node ()
{
	assert (Representation :: is_initialized ());

	btVector3 origin = get_position ();
	btQuaternion rot = get_motion_state () -> m_graphicsWorldTrans . getRotation ();
	node . setPosition (origin . x (), origin . y (), origin . z ());
	node . setOrientation (rot . getW (), rot . x (), rot . y (), rot . z ());
}

//	private
btDefaultMotionState * Representation ::
	get_motion_state () const
{
	assert (Representation :: is_initialized ());

	btDefaultMotionState * result = dynamic_cast <btDefaultMotionState *> (const_cast <Representation *> (this) -> getMotionState ());
	assert (result != NULL);
	return result;
}

void Representation ::
	move (float distance)
{
	assert (Representation :: is_initialized ());

	trace () << "(absolute) position: (" << get_position () . getX () << ", " << get_position () . getY () << ", " << get_position () . getZ () << ")" << endl;

	Ogre :: Quaternion r = Ogre :: Quaternion
	(
		get_rotation () . getW (),
		get_rotation () . getX (),
		get_rotation () . getY (),
		get_rotation () . getZ ()
	);
	
	Ogre :: Vector3 v = Ogre :: Vector3 (0, 0, - distance);

	//	This is not some kind of multiplication, but the rotation of r applied to v.
	//	Bullet seems to lack equivalent functionality.
	Ogre :: Vector3 w = r * v;	
	trace () << "new (relative) position: (" << w . x << ", " << w . y << ", " << w . z << ")" << endl;
	set_position (get_position () + btVector3 (w . x, w . y, w . z));
	trace () << "new (absolute) position: (" << get_position () . getX () << ", " << get_position () . getY () << ", " << get_position () . getZ () << ")" << endl;

	update_scene_node ();
}

void Representation ::
	turn (float radian_angle)
{
	assert (Representation :: is_initialized ());

	trace () << "(absolute) rotation: (" << get_rotation () . getX () << ", " << get_rotation () . getY () << ", " << get_rotation () . getZ () << ")" << endl;

//	trace () << "new (relative) rotation: (" << get_rotation () . getX () << ", " << get_rotation () . getY () << ", " << get_rotation () . getZ () << ")" << endl;
	
	set_rotation (get_rotation () * btQuaternion (btVector3 (0, 1, 0), radian_angle));

	trace () << "new (absolute) rotation: (" << get_rotation () . getX () << ", " << get_rotation () . getY () << ", " << get_rotation () . getZ () << ")" << endl;

}

btVector3 & tsl :: to_btVector3 (Ogre :: Vector3 old)
{
	return * (new btVector3 (old . x, old . y, old . z));
}
