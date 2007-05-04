#include "body.hpp"

using namespace std;
using namespace tsl;

//  constructor
Body ::
	Body (Item & new_item, Ogre :: Vector3 position, float scale, OgreOde :: Geometry & new_geometry) :
	Object (new_item + "'s body"),
	Disjoint_Set <Item> (1),
	item (new_item),
	node (* Environment :: get () . root_node . createChildSceneNode (string :: data ())),
	geometry (new_geometry)
{
	log (debugging) << get_class_name () << " (" << new_item << ", " << to_string (position) << ", " << scale << ")" << endl;
	assert (Disjoint_Set <Item> :: is_initialized ());

	Disjoint_Set <Item> :: add (item);
	seal ();

//	Environment :: get () . root_node . addChild (this);

	node . setPosition (position);
	node . setScale (scale, scale, scale);
	node . attachObject (& item . entity);

	assert (node . numAttachedObjects () == 1);
	
	item . entity . setUserObject (& geometry);

	if (geometry . getBody () == NULL)
	{
		log (debugging) << "I'm a static body." << endl;

		geometry . setUserObject (& item . entity);
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

	return true;
}

//	static
string Body ::
	get_class_name ()
{
	return "Body";
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

	item . entity . setMaterialName (name);
}

void Body ::
	set_space (OgreOde :: Space & new_space)
{
	OgreOde :: Space * old_space = geometry . getSpace ();
	if (old_space != NULL)
	{
		old_space -> removeGeometry (geometry);
	}
	new_space . addGeometry (geometry);
}

Ogre :: Quaternion tsl :: make_quaternion (float radian_angle, Ogre :: Vector3 ax)
{
	return Ogre :: Quaternion (Ogre :: Radian (radian_angle), ax);
}
