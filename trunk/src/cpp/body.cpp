#include "body.hpp"

using namespace std;
using namespace tsl;

//  constructor
Body ::
	Body
	(
		Ogre :: SceneManager & scene_manager,
		Observable <Body> & observer,
		Ogre :: Entity & new_entity,
		OgreOde :: Geometry & new_geometry
	) :
	Object (observer + "'s body"),
	Ogre :: SceneNode (& scene_manager, string :: data ()),
	entity (new_entity),
	geometry (new_geometry),
	root_node (scene_manager . getRootSceneNode ())
{
	log (TSL_DEBUG) << get_class_name () << " (~scene_manager~, " << observer << ", ~new_entity~, ~new_geometry~)" << endl;
	assert (Object :: is_initialized ());

	scene_manager . getRootSceneNode () -> addChild (this);
	Ogre :: SceneNode :: setParent (scene_manager . getRootSceneNode ());

	attachObject (& entity);
	
	assert (numAttachedObjects () == 1);
	
	entity . setUserObject (& geometry);

	body = geometry . getBody ();

	if (body != NULL)
	{
		attachObject (body);
	}

	/*TODO observer -> ...*/

	assert (Body :: is_initialized ());
}

//  destructor
Body ::
	~Body ()
{
	log (TSL_DEBUG) << "~" << get_class_name () << " ()" << endl;
	assert (Body :: is_initialized ());
	log (TSL_DEBUG) << "~" << get_class_name () << " () A" << endl;
	
	getParent () -> removeChild (Ogre :: Node :: getName ());

	log (TSL_DEBUG) << "~" << get_class_name () << " () C" << endl;
	if (is_mobile ())
	{
	log (TSL_DEBUG) << "~" << get_class_name () << " () D" << endl;
		detachObject (body);
	log (TSL_DEBUG) << "~" << get_class_name () << " () E" << endl;
		delete body;
	log (TSL_DEBUG) << "~" << get_class_name () << " () F" << endl;
		body = NULL;
	log (TSL_DEBUG) << "~" << get_class_name () << " () G" << endl;
	}
	log (TSL_DEBUG) << "~" << get_class_name () << " () H" << endl;
	
	assert (numAttachedObjects () == 1);
	log (TSL_DEBUG) << "~" << get_class_name () << " () I" << endl;

	int zero = 0;
	log (TSL_DEBUG) << "~" << get_class_name () << " () J" << endl;
	getCreator () -> destroyMovableObject (detachObject (zero));
	log (TSL_DEBUG) << "~" << get_class_name () << " () K" << endl;

	assert (numAttachedObjects () == 0);
	log (TSL_DEBUG) << "~" << get_class_name () << " () L" << endl;

	assert (Object :: is_initialized ());
}

//	virtual
bool Body ::
	is_initialized ()
	const
{
	assert (warn <Body> (Object :: is_initialized ()));
	assert (Ogre :: SceneNode :: getParent () == root_node);
	assert (Ogre :: SceneNode :: numAttachedObjects () <= 2);

	return true;
}

//	static
string Body ::
	get_class_name ()
{
	return "Body";
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
	return getOrientation () * - z_axis;
}

Ogre :: Vector3 Body ::
	get_side_direction () const
{
	assert (Body :: is_initialized ());

	return getOrientation () * x_axis;
}

Ogre :: Vector3 Body ::
	get_top_direction () const
{
	assert (Body :: is_initialized ());

	return getOrientation () * y_axis;
}

float Body ::
	get_scale ()
{
	assert (Body :: is_initialized ());

	return Ogre :: Math :: Sqrt (getScale () . squaredLength () / 3);
}

void Body ::
	set_scale (float scale)
{
	assert (Body :: is_initialized ());

	//	TODO set the scale for OgreOde to.

	setScale (scale, scale, scale);
}

void Body ::
	set_material (string name)
{
	assert (Body :: is_initialized ());

	entity . setMaterialName (name);
}

void Body ::
	move (float distance, Ogre :: Vector3 ax)
{
	assert (Body :: is_initialized ());

	if (ax == zero)
	{
		ax = get_front_direction ();
	}

	setPosition (getPosition () + distance * ax);
	//	addForce (distance * get_front_direction ());
	
	log (TSL_DEBUG) << "new position: " << to_string (getPosition ()) << endl;
}

void Body ::
	turn (float radian_angle, Ogre :: Vector3 ax)
{
	log (TSL_DEBUG) << "turn (" << radian_angle << ", " << to_string (ax) << ")" << endl;
	assert (Body :: is_initialized ());

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
