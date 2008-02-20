/*
	static scene item
	- model (always)
	- body: static

	character
	- model (always)
	- body: dynamic

	inventory item
	- model: sometimes
	- body: sometimes - dynamic
*/

#include "body.hpp"
#include "engines/log.hpp"
#include "items/item.hpp"
#include "model.hpp"

#include <OgreSceneNode.h>

using namespace std;
using namespace Radakan;
using namespace Radakan :: Items;

//	static
string Item ::
	get_class_name ()
{
	return "Items :: Item";
}

//  constructor
Item ::
	Item
	(
		float new_mass,
		Mathematics :: Vector_3D new_size,
		const Reference <Mesh_Data> new_mesh_data
	) :
	Object ("The name doesn't matter is this is an abstact base class."),
	mass (new_mass),
	size (new_size),
	solid (true),
	containable (true),
	mesh_data (new_mesh_data)
{
	Engines :: Log :: trace
	(
		me,
		Item :: get_class_name (),
		"",
		to_string (mass),
		new_size . to_string (),
		mesh_data . get_name ()
	);
	assert (0 <= mass);
	assert (0 <= size . x);
	assert (0 <= size . y);
	assert (0 <= size . z);
	if (mesh_data . points_to_object ())
	{
		assert (mesh_data -> is_initialized ());
	}

	//	Do nothing.

	assert (Item :: is_initialized ());
}

//  destructor
Item ::
	~Item ()
{
	Engines :: Log :: trace (me, Item :: get_class_name (), "~");
	assert (Item :: is_initialized ());
	
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
}

//	virtual
bool Item ::
	is_initialized ()
	const
{
	//	Engines :: Log :: trace (me, Item :: get_class_name (), "is_initialized");
	assert (Object :: is_initialized ());
	assert (0 <= mass);
	assert (0 <= size . x);
	assert (0 <= size . y);
	assert (0 <= size . z);
	if (mesh_data . points_to_object ())
	{
		assert (mesh_data -> is_initialized ());
	}
	
	return true;
}

//	virtual
float Item ::
	get_total_mass ()
	const
{
	assert (Item :: is_initialized ());
	
	return mass;
}

void Item ::
	appear (Mathematics :: Vector_3D position, Mathematics :: Quaternion orientation)
{
	Engines :: Log :: trace (me, Item :: get_class_name (), "appear");
	
	assert (Item :: is_initialized ());
	assert (! model . points_to_object ());

	model . reset_pointee
		(Reference <Model> (new Model (mesh_data, position, orientation, size)));

	body . reset_pointee (Body :: create (mass, size, position, orientation));
}

void Item ::
	disappear ()
{
	assert (Item :: is_initialized ());
	assert (model . points_to_object ());

	body . reset_pointee ();
	model . reset_pointee ();
}

Reference <Body> Item ::
	get_body () const
{
	assert (Item :: is_initialized ());
	
	return body;
}

float Item ::
	get_volume () const
{
	assert (Item :: is_initialized ());

	return size . x * size . y * size . z;
}

#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
void Item ::
	sync_model ()
{
	assert (body . points_to_object ());
	assert (model . points_to_object ());

	model -> set_position (body -> get_position ());
	model -> set_orientation (body -> get_orientation ());
}
#endif
