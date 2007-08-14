#include "log.hpp"
#include "model.hpp"
#include "world.hpp"

using namespace std;
using namespace Radakan;

//	static
const string Model ::
	get_class_name ()
{
	return "Model";
}

//  constructor
Model ::
	Model (Items :: Item & new_item, Ogre :: Vector3 position, float scale, OgreOde :: Geometry & new_geometry) :
	Object (new_item . name + "'s model"),
	Location <Items :: Item> (1),
	item (new_item),
	node (* World :: get () . root_node . createChildSceneNode (name)),
	geometry (new_geometry)
{
	Engines :: Log :: trace (me, Model :: get_class_name (), "", new_item . name, to_string (position), to_string (scale));

	add (item);
	seal ();

//	World :: get () . root_node . addChild (this);

	node . setPosition (position);
	node . setScale (scale, scale, scale);
	node . attachObject (& item . entity);

	assert (node . numAttachedObjects () == 1);
	
	item . entity . setUserObject (& geometry);

	if (geometry . getBody () == NULL)
	{
		Engines :: Log :: log (me) << "I'm a static model." << endl;

		geometry . setUserObject (& item . entity);
		geometry . setPosition (position);
	}

	item . set_model (* this);

	if (! item . solid)
	{
		geometry . disable ();
	}

	assert (Model :: is_initialized ());
}

//  destructor
Model ::
	~Model ()
{
	Engines :: Log :: trace (me, Model :: get_class_name (), "~");
	assert (Model :: is_initialized ());

	prepare_for_destruction ();

	item . remove_model ();
	
	assert (node . numAttachedObjects () == 1);

	World :: get () . getSceneManager () -> destroyMovableObject (& item . entity);

	assert (node . numAttachedObjects () == 0);

	node . getParent () -> removeChild (node . getName ());
}

//	virtual
bool Model ::
	is_initialized ()
	const
{
	assert (Set <Items :: Item> :: is_initialized ());
	//	TODO re-enable:
	//	assert (is_sealed ());
	assert (item . has_model ());
	assert (node . getParent () == & World :: get () . root_node);
	assert (node . numAttachedObjects () <= 2);

	//	TODO re-enable:
	//	assert ((node . getPosition () - geometry . getPosition ()) . length () < 0.01);

	return true;
}

Ogre :: Vector3 Model ::
	get_front_direction () const
{
	assert (Model :: is_initialized ());

	//	notice the minus sign
	return node . getOrientation () * - z_axis;
}

Ogre :: Vector3 Model ::
	get_side_direction () const
{
	assert (Model :: is_initialized ());

	return node . getOrientation () * x_axis;
}

Ogre :: Vector3 Model ::
	get_top_direction () const
{
	assert (Model :: is_initialized ());

	return node . getOrientation () * y_axis;
}

void Model ::
	set_material (string name)
{
	assert (Model :: is_initialized ());

	item . entity . setMaterialName (name);
}

void Model ::
	set_space (OgreOde :: Space & new_space)
{
	OgreOde :: Space * old_space = geometry . getSpace ();
	if (old_space != NULL)
	{
		old_space -> removeGeometry (geometry);
	}
	new_space . addGeometry (geometry);
}

Ogre :: Quaternion Radakan :: make_quaternion (float radian_angle, Ogre :: Vector3 ax)
{
	return Ogre :: Quaternion (Ogre :: Radian (radian_angle), ax);
}
