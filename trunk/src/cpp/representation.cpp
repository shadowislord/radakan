#include "item.hpp"

using namespace std;
using namespace tsl;

//  constructor
Representation ::
	Representation (Item & new_item, OgreOde :: World & world) :
	Object (new_item + "'s representation"),
	Ogre :: SceneNode (world . getSceneManager (), string :: data ()),
	item (new_item)
{
	trace () << get_class_name () << " (" << new_item << ", ~world~)" << endl;
	assert (Object :: is_initialized ());

	world . getSceneManager () -> getRootSceneNode () -> addChild (this);

	entity = world . getSceneManager () -> createEntity
										(item + "'s entity", item . mesh_name);
	attachObject (entity);
	
	assert (numAttachedObjects () == 1);
	
	if (find ("Ground") != string :: npos)
	{
		OgreOde :: EntityInformer * ei = new OgreOde :: EntityInformer (entity, _getFullTransform ());
		geometry = ei -> createStaticTriangleMesh (& world, world . getDefaultSpace ());
		trace () << "A static triangle mesh was created for " << * this << "." << endl;

		body = NULL;
	}
	else
	{
		body = new OgreOde :: Body (& world, string :: data ());

		attachObject (body);

		geometry = new OgreOde :: SphereGeometry (Ogre :: Math :: RangeRandom (0.5, 1.5), & world, world . getDefaultSpace ());
//		geometry -> setOffsetPosition (Ogre :: Vector3 (0, 1.5, 0));
		trace () << "A default sphere mesh was created for " << * this << "." << endl;

		geometry -> setBody (body);

		body -> setMass (OgreOde :: SphereMass (item . mass, 1 /*TODO set the right radius*/));

	}
	entity -> setUserObject (geometry);

	assert (Representation :: is_initialized ());
}

//  destructor
Representation ::
	~Representation ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (Representation :: is_initialized ());
	trace () << "~" << get_class_name () << " () A" << endl;
	
	getParent () -> removeChild (this);

	trace () << "~" << get_class_name () << " () C" << endl;
	if (is_mobile ())
	{
	trace () << "~" << get_class_name () << " () D" << endl;
		detachObject (body);
	trace () << "~" << get_class_name () << " () E" << endl;
		delete body;
	trace () << "~" << get_class_name () << " () F" << endl;
		body = NULL;
	trace () << "~" << get_class_name () << " () G" << endl;
	}
	trace () << "~" << get_class_name () << " () H" << endl;
	
	assert (numAttachedObjects () == 1);
	trace () << "~" << get_class_name () << " () I" << endl;

	int zero = 0;
	trace () << "~" << get_class_name () << " () J" << endl;
	getCreator () -> destroyMovableObject (detachObject (zero));
	trace () << "~" << get_class_name () << " () K" << endl;

	assert (numAttachedObjects () == 0);
	trace () << "~" << get_class_name () << " () L" << endl;

	assert (Object :: is_initialized ());
}

//	virtual
bool Representation ::
	is_initialized ()
	const
{
	assert (warn <Representation> (Object :: is_initialized ()));
	assert (Ogre :: SceneNode :: numAttachedObjects () <= 2);
	assert (Ogre :: SceneNode :: getParent () != NULL);

	return true;
}

//	static
string Representation ::
	get_class_name ()
{
	return "Representation";
}

bool Representation ::
	is_mobile () const
{
	return (body != NULL);
}

Ogre :: Vector3 Representation ::
	get_front_direction () const
{
	assert (Representation :: is_initialized ());

	//	notice the minus sign
	return getOrientation () * - z_axis;
}

Ogre :: Vector3 Representation ::
	get_side_direction () const
{
	assert (Representation :: is_initialized ());

	return getOrientation () * x_axis;
}

Ogre :: Vector3 Representation ::
	get_top_direction () const
{
	assert (Representation :: is_initialized ());

	return getOrientation () * y_axis;
}

float Representation ::
	get_scale ()
{
	assert (Representation :: is_initialized ());

	return Ogre :: Math :: Sqrt (getScale () . squaredLength () / 3);
}

void Representation ::
	set_scale (float scale)
{
	assert (Representation :: is_initialized ());

	//	TODO set the scale for OgreOde to.

	setScale (scale, scale, scale);
}

void Representation ::
	set_material (string name)
{
	assert (Representation :: is_initialized ());

	entity -> setMaterialName (name);
}

void Representation ::
	move (float distance, Ogre :: Vector3 ax)
{
	assert (Representation :: is_initialized ());

	if (ax == zero)
	{
		ax = get_front_direction ();
	}

	setPosition (getPosition () + distance * ax);
	//	addForce (distance * get_front_direction ());
	
	debug () << "new position: " << to_string (getPosition ()) << endl;
}

void Representation ::
	turn (float radian_angle, Ogre :: Vector3 ax)
{
	debug () << "turn (" << radian_angle << ", " << to_string (ax) << ")" << endl;
	assert (Representation :: is_initialized ());

	if (ax == zero)
	{
		ax = get_top_direction ();
	}

	setOrientation (getOrientation () * Ogre :: Quaternion (Ogre :: Radian (radian_angle), ax));
}

Ogre :: Quaternion tsl :: make_quaternion (float radian_angle, Ogre :: Vector3 ax)
{
	return Ogre :: Quaternion (Ogre :: Radian (radian_angle), ax);
}
