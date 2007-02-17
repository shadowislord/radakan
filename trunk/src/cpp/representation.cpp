#include "representation.hpp"

using namespace std;
using namespace tsl;

//  constructor
Representation ::
	Representation (string item_name, Ogre :: SceneNode & new_node, OgreOde :: World * world) :
	Object (item_name + "'s representation"),
	OgreOde :: Body (world, item_name + "'s representation"),
//	parent_item (new_parent_item),
	node (new_node)
{
	trace () << get_class_name () << " ()" << endl;
	assert (Object :: is_initialized ());

	node . attachObject (this);

	geometry = new OgreOde :: SphereGeometry (10 /*size . x*/, world, world -> getDefaultSpace ());
	geometry -> setBody (this);
	get_entity () . setUserObject (geometry);

	assert (Representation :: is_initialized ());
}

//  destructor
Representation ::
	~Representation ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (Representation :: is_initialized ());

	node . detachObject (this);

	assert (node . numAttachedObjects () == 1);

	int zero = 0;
	node . getCreator () -> destroyMovableObject (node . detachObject (zero));

	assert (node . numAttachedObjects () == 0);

	assert (Object :: is_initialized ());
}

//	virtual
bool Representation ::
	is_initialized ()
	const
{
	assert (warn <Representation> (Object :: is_initialized ()));
	assert (node . numAttachedObjects () == 2);

	return true;
}

//	static
string Representation ::
	get_class_name ()
{
	return "Representation";
}

Ogre :: Vector3 Representation ::
	get_front_direction () const
{
	assert (Representation :: is_initialized ());

	return getOrientation () * Ogre :: Vector3 (0, 0, 1);
}

Ogre :: Vector3 Representation ::
	get_side_direction () const
{
	assert (Representation :: is_initialized ());

	return getOrientation () * Ogre :: Vector3 (1, 0, 0);
}

Ogre :: Vector3 Representation ::
	get_top_direction () const
{
	assert (Representation :: is_initialized ());

	return getOrientation () * Ogre :: Vector3 (0, 1, 0);
}

void Representation ::
	set_scale (float scale)
{
	assert (Representation :: is_initialized ());

	//	TODO set the scale for bullet to.

	node . setScale (scale, scale, scale);
}

void Representation ::
	move (float distance)
{
	assert (Representation :: is_initialized ());

	setPosition (getPosition () - distance * get_front_direction ());
	
	debug () << "body position: (" << getPosition () . x << ", " << getPosition () . y << ", " << getPosition () . z << ")" << endl;
	debug () << "node position: (" << node . getPosition () . x << ", " << node . getPosition () . y << ", " << node . getPosition () . z << ")" << endl;
}

void Representation ::
	turn (float radian_angle, Ogre :: Vector3 ax)
{
	debug () << "turn (" << radian_angle << ", (" << ax . x << ", " << ax . y << ", " << ax . z << "))" << endl;
	assert (Representation :: is_initialized ());

	setOrientation (getOrientation () * Ogre :: Quaternion (Ogre :: Radian (radian_angle), ax));
}

Ogre :: Entity & Representation ::
	get_entity () const
{
	assert (Representation :: is_initialized ());

	if (dynamic_cast <Ogre :: Entity *> (node . getAttachedObject (0)) == NULL)
	{
		return * dynamic_cast <Ogre :: Entity *> (node . getAttachedObject (1));
	}
	return * dynamic_cast <Ogre :: Entity *> (node . getAttachedObject (0));
}

Ogre :: Quaternion tsl :: make_quaternion (float radian_angle, Ogre :: Vector3 ax)
{
	return Ogre :: Quaternion (Ogre :: Radian (radian_angle), ax);
}
