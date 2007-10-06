#include <OgreOdeGeometry.h>
#include <OgreOdeSpace.h>
#include <OgreOdeBody.h>
#include <OgreOdeWorld.h>

#include "item.hpp"
#include "log.hpp"
#include "model.hpp"
#include "slot.hpp"
#include "world.hpp"

using namespace std;
using namespace Radakan;

//	static
string Model ::
	get_class_name ()
{
	return "Model";
}

//  constructor
Model ::
	Model (Reference <Items :: Item> new_item, Ogre :: Vector3 position, float scale) :
	Object (new_item -> name + "'s model"),
	Location <Items :: Item> (1),	//	A Model corresponds to a single Item.
	item (new_item),
	node (World :: get () -> root_node -> createChildSceneNode (name)),
	geometry (create_geometry (item)),
	entity
	(
		World :: get () -> ogre_ode_world -> getSceneManager () -> createEntity
		(
			name + "'s entity", item -> mesh_name
		)
	)
{
	Engines :: Log :: trace (me, Model :: get_class_name (), "", new_item -> name, to_string (position), to_string (scale));

	add (item);
	seal ();

//	World :: get () . root_node . addChild (this);

	node -> setPosition (position);
	node -> setScale (scale, scale, scale);
	node -> attachObject (entity . get ());

	assert (node -> numAttachedObjects () == 1);
	
	entity -> setUserObject (geometry . get ());

	if (item -> mobile)
	{
		Engines :: Log :: log (me) << "I'm a static model." << endl;

		geometry -> setUserObject (entity . get ());
		geometry -> setPosition (position);
	}

	item -> set_model (Reference <Model> (this));

	if (! item -> solid)
	{
		geometry -> disable ();
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

	item -> remove_model ();
	
	assert (node -> numAttachedObjects () == 1);

	World :: get () -> ogre_ode_world -> getSceneManager () -> destroyMovableObject (entity . get ());

	assert (node -> numAttachedObjects () == 0);

	node -> getParent () -> removeChild (node -> getName ());
}

//	virtual
bool Model ::
	is_initialized ()
	const
{
	assert (this -> Location <Items :: Item> :: is_initialized ());
	//	TODO re-enable:
	//	assert (is_sealed ());
	assert (node -> getParent () != NULL);
	assert (World :: get () -> root_node . get () != NULL);
	
	assert (node -> getParent () == World :: get () -> root_node . get ());
	assert (node -> numAttachedObjects () <= 2);

	//	TODO re-enable:
	//	assert ((node . getPosition () - geometry . getPosition ()) . length () < 0.01);

	return true;
}

Ogre :: Vector3 Model ::
	get_front_direction () const
{
	assert (Model :: is_initialized ());

	//	notice the minus sign
	return - node -> getOrientation () * z_axis;
}

Ogre :: Vector3 Model ::
	get_side_direction () const
{
	assert (Model :: is_initialized ());

	return node -> getOrientation () * x_axis;
}

Ogre :: Vector3 Model ::
	get_top_direction () const
{
	assert (Model :: is_initialized ());

	return node -> getOrientation () * y_axis;
}

void Model ::
	set_material (string name)
{
	assert (Model :: is_initialized ());

	entity -> setMaterialName (name);
}

void Model ::
	set_space (boost :: shared_ptr <OgreOde :: Space> new_space)
{
	/*boost :: shared_ptr <OgreOde :: Space> old_space (geometry -> getSpace ());
	if (old_space)
	{
		old_space -> removeGeometry (* geometry . get ());
	}*/
	new_space -> addGeometry (* geometry . get ());
}

Ogre :: Quaternion Radakan :: make_quaternion (float radian_angle, Ogre :: Vector3 ax)
{
	return Ogre :: Quaternion (Ogre :: Radian (radian_angle), ax);
}

boost :: shared_ptr <OgreOde :: Geometry> Radakan :: create_geometry
	(Reference <Items :: Item> item)
{
	assert (item . points_to_object ());
	assert (item -> is_initialized ());
	assert (! item -> has_model ());

	boost :: shared_ptr <OgreOde :: Geometry> geometry (new OgreOde :: BoxGeometry (item -> size, World :: get () -> ogre_ode_world . get ()));

	return geometry;
}
