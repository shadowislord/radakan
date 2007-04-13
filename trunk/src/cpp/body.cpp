#include "body.hpp"

using namespace std;
using namespace tsl;

//  constructor
Body ::
	Body (Item & new_item, Ogre :: Vector3 position, float scale) :
	Object (new_item + "'s body"),
	Disjoint_Set <Item> (1),
	item (new_item),
	node (* Environment :: get () . root_node . createChildSceneNode (string :: data ())),
	entity (item . entity),
	geometry (item . create_geometry ())
{
	log (debugging) << get_class_name () << " (" << new_item << ", " << to_string (position) << ", " << scale << ")" << endl;
	assert (Disjoint_Set <Item> :: is_initialized ());

	Disjoint_Set <Item> :: add (item);
	seal ();

//	Environment :: get () . root_node . addChild (this);

	node . setPosition (position);
	node . setScale (scale, scale, scale);
	node . attachObject (& entity);

	assert (node . numAttachedObjects () == 1);
	
	body = geometry . getBody ();

	entity . setUserObject (& geometry);

	if (body != NULL)
	{
		log (debugging) << "I'm a mobile body." << endl;

		node . attachObject (body);
	}
	else
	{
		log (debugging) << "I'm a static body." << endl;

		geometry . setUserObject (& entity);
		geometry . setPosition (position);
	}

	item . set_body (* this);

	if (! item . solid)
	{
		geometry . disable ();
	}

	assert (Body :: is_initialized ());
}

//  destructor
Body ::
	~Body ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;
	assert (Body :: is_initialized ());
	
	item . remove_body ();
	
	node . getParent () -> removeChild (node . getName ());

	if (is_mobile ())
	{
		node . detachObject (body);
		delete body;
		body = NULL;
	}
	
	assert (node . numAttachedObjects () == 1);

	unsigned int zero = 0;
	node . getCreator () -> destroyMovableObject (node . detachObject (zero));

	assert (node . numAttachedObjects () == 0);

	assert (Disjoint_Set <Item> :: is_initialized ());
}

//	virtual
bool Body ::
	is_initialized ()
	const
{
	assert (Disjoint_Set <Item> :: is_initialized ());
	assert (is_sealed ());
	assert (item . has_body ());
	assert (node . getParent () == & Environment :: get () . root_node);
	assert (node . numAttachedObjects () <= 2);

	//	TODO re-enable assert ((node . getPosition () - geometry . getPosition ()) . length () < 0.01);
	if (body != NULL)
	{
		//	TODO re-enable assert ((body -> getPosition () - node . getPosition ()) . length () < 0.01);
		//	TODO re-enable assert ((body -> getPosition () - geometry . getPosition ()) . length () < 0.01);
	}

	return true;
}

//	static
string Body ::
	get_class_name ()
{
	return "Body";
}

//	virtual
bool Body ::
	add (Item & item)
{
	assert (Body :: is_initialized ());
	assert (item . is_initialized ());

	return false;
}

//	virtual
bool Body ::
	move (Item & item, Disjoint_Set <Item> & destination)
{
	assert (Body :: is_initialized ());
	assert (item . is_initialized ());
	assert (destination . is_initialized ());
	
	return false;
}

bool Body ::
	is_mobile () const
{
	return (body != NULL);
}

Ogre :: Vector3 Body ::
	get_front_direction () const
{
	assert (Body :: is_initialized ());

	//	notice the minus sign
	return node . getOrientation () * - z_axis;
}

Ogre :: Vector3 Body ::
	get_side_direction () const
{
	assert (Body :: is_initialized ());

	return node . getOrientation () * x_axis;
}

Ogre :: Vector3 Body ::
	get_top_direction () const
{
	assert (Body :: is_initialized ());

	return node . getOrientation () * y_axis;
}

void Body ::
	set_material (string name)
{
	assert (Body :: is_initialized ());

	entity . setMaterialName (name);
}

void Body ::
	move (float top_speed, float turn_length)
{
	assert (Body :: is_initialized ());
	assert (is_mobile ());
	assert (Ogre :: Math :: Abs (top_speed) <= 1);
	assert (0 <= turn_length);

	log (debugging) << "Position: " << to_string (body -> getPosition ()) << endl;
	log (debugging) << "Speed: " << to_string (body -> getPointWorldVelocity (body -> getPosition ())) << endl;
	
	log (debugging) << "current force: " << to_string (body -> getForce ()) << endl;
	
	body -> setForce (200 * (top_speed * get_front_direction () - body -> getLinearVelocity ()));
	
	log (debugging) << "new force: " << to_string (body -> getForce ()) << endl;
}

void Body ::
	turn (float top_radian_angle_speed, float turn_length, Ogre :: Vector3 ax)
{
	assert (Body :: is_initialized ());
	assert (is_mobile ());
	assert (Ogre :: Math :: Abs (top_radian_angle_speed) <= 1);
	assert (0 <= turn_length);

	if (ax == zero_vector)
	{
		ax = get_top_direction ();
	}

	body -> setTorque (10 * (top_radian_angle_speed * ax - body -> getAngularVelocity ()));
}

void Body ::
	reset ()
{
	log (debugging) << "reset ()" << endl;
	assert (Body :: is_initialized ());
	assert (is_mobile ());

	body -> setOrientation (Ogre :: Quaternion (1, 0, 0, 0));
	body -> setPosition (body -> getPosition () + y_axis * (2 - body -> getPosition () . y));
	body -> setLinearVelocity (zero_vector);
	body -> setForce (zero_vector);
}

Ogre :: Quaternion tsl :: make_quaternion (float radian_angle, Ogre :: Vector3 ax)
{
	return Ogre :: Quaternion (Ogre :: Radian (radian_angle), ax);
}
