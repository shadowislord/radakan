#include "item.hpp"

using namespace std;
using namespace tsl;

//  constructor
Representation ::
	Representation (Item & new_item, OgreOde :: World & world) :
	Object (new_item + "'s representation"),
	OgreOde :: Body (& world, string :: data ()),
	item (new_item)
{
	trace () << get_class_name () << " (" << new_item << ", ~world~)" << endl;
	assert (Object :: is_initialized ());

	node = world . getSceneManager () -> getRootSceneNode () -> createChildSceneNode ();

	Ogre :: Entity * entity = world . getSceneManager () -> createEntity
										(item + "'s entity", item . mesh_name);
	node -> attachObject (entity);
	
	assert (node -> numAttachedObjects () == 1);
	assert (node -> getAttachedObject (0) != NULL);
	
	node -> attachObject (this);

	geometry = new OgreOde :: SphereGeometry (10 * Ogre :: Math :: RangeRandom (0.5, 1.5) /*size . x*/, & world, world . getDefaultSpace ());
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

	node -> detachObject (this);

	assert (node -> numAttachedObjects () == 1);

	int zero = 0;
	node -> getCreator () -> destroyMovableObject (node -> detachObject (zero));

	assert (node -> numAttachedObjects () == 0);

	assert (Object :: is_initialized ());
}

//	virtual
bool Representation ::
	is_initialized ()
	const
{
	assert (warn <Representation> (Object :: is_initialized ()));
	assert (node -> numAttachedObjects () == 2);

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

	node -> setScale (scale, scale, scale);
}

void Representation ::
	move (float distance)
{
	assert (Representation :: is_initialized ());

	setPosition (getPosition () - distance * get_front_direction ());
	//	addForce (- distance * get_front_direction ());
	
	debug () << "new OgreOde :: Body position: " << to_string (getPosition ()) << endl;
	debug () << "new Ode :: Body position: (" << dBodyGetPosition (_body) [0] << ", " << dBodyGetPosition (_body) [1] << ", " << dBodyGetPosition (_body) [2] << ")" << endl;
	debug () << "new Ogre :: SceneNode position: " << to_string (node -> getPosition ()) << endl;
}

void Representation ::
	turn (float radian_angle, Ogre :: Vector3 ax)
{
	debug () << "turn (" << radian_angle << ", " << to_string (ax) << ")" << endl;
	assert (Representation :: is_initialized ());

	setOrientation (getOrientation () * Ogre :: Quaternion (Ogre :: Radian (radian_angle), ax));
}

Ogre :: Entity & Representation ::
	get_entity () const
{
	assert (Representation :: is_initialized ());

	if (dynamic_cast <Ogre :: Entity *> (node -> getAttachedObject (0)) == NULL)
	{
		return * dynamic_cast <Ogre :: Entity *> (node -> getAttachedObject (1));
	}
	return * dynamic_cast <Ogre :: Entity *> (node -> getAttachedObject (0));
}

Ogre :: Quaternion tsl :: make_quaternion (float radian_angle, Ogre :: Vector3 ax)
{
	return Ogre :: Quaternion (Ogre :: Radian (radian_angle), ax);
}
