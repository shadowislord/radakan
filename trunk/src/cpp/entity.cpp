#include "entity.hpp"

using namespace std;
using namespace tsl;

//  constructor
Entity ::
	Entity
	(
		bool new_movable,
		bool new_solid,
		bool new_visible,
		float new_volume,
		float new_weight,
		btVector3 new_position,
		Ogre :: SceneNode & new_node
	) :
	Object (get_name (new_node)),
	btRigidBody
	(
		1,
		new btDefaultMotionState
		(
			btTransform (btQuaternion (0, 0, 0, 1), new_position)
		),
		new btSphereShape (10)
	),
	movable (new_movable),
	solid (new_solid),
	volume (new_volume),
	weight (new_weight)
{
	assert (Object :: is_initialized ());

	node = & new_node;
	node -> setVisible (new_visible);

	update_scene_node ();

	assert (Entity :: is_initialized ());
}

//  destructor
Entity ::
	~Entity ()
{
	trace () << "~Entity ()" << endl;
	assert (Entity :: is_initialized ());
	
	assert (Object :: is_initialized ());
}

//	virtual
bool Entity ::
	is_initialized ()
	const
{
	return warn <Entity> (Object :: is_initialized () && (0 <= volume) && (0 <= weight) && (node != NULL));
}

//	static
string Entity ::
	get_class_name ()
{
	return "Entity";
}

//	virtual
float Entity ::
	get_total_weight ()
	const
{
	trace () << "get_total_weight ()" << endl;
	assert (Entity :: is_initialized ());
	
	return weight;
}

btQuaternion Entity ::
	get_rotation  () const
{
	assert (Entity :: is_initialized ());

	return get_motion_state () -> m_graphicsWorldTrans . getRotation ();
}

void Entity ::
	set_rotation  (const btQuaternion & new_rotation)
{
	assert (Entity :: is_initialized ());

	//	!!!	This sets the absolute rotation, instead of the relative rotation (to the current rotation).
	get_motion_state () -> m_graphicsWorldTrans . setRotation (new_rotation);
	update_scene_node ();
}

btVector3 Entity ::
	get_position () const
{
	assert (Entity :: is_initialized ());

	return get_motion_state () -> m_graphicsWorldTrans . getOrigin ();
}

void Entity ::
	set_position (const btVector3 & new_position)
{
	assert (Entity :: is_initialized ());

	get_motion_state () -> m_graphicsWorldTrans . setOrigin (new_position);
	
//	This somehow doesn't do what it's supposed to do. (It doesn't change anything.)
//	translate (new_position);

//	Keep the Ogre :: SceneNode in sync.
	update_scene_node ();
}

void Entity ::
	update_scene_node ()
{
	assert (Entity :: is_initialized ());

	btVector3 origin = get_position ();
	btQuaternion rot = get_motion_state () -> m_graphicsWorldTrans . getRotation ();
	node -> setPosition (origin . x (), origin . y (), origin . z ());
	node -> setOrientation (rot . getW (), rot . x (), rot . y (), rot . z ());
}

//	private
btDefaultMotionState * Entity ::
	get_motion_state () const
{
	assert (Entity :: is_initialized ());

	btDefaultMotionState * result = dynamic_cast <btDefaultMotionState *> (const_cast <Entity *> (this) -> getMotionState ());
	assert (result != NULL);
	return result;
}

string tsl :: get_name (Ogre :: SceneNode & node)
{
	assert (node . numAttachedObjects () == 1);
	Ogre :: MovableObject * movable_object = node . getAttachedObject (0);
	assert (movable_object != NULL);

	return movable_object -> getName ();
}
