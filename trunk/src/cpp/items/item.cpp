#include "engines/log.hpp"
#include "items/item.hpp"
#include "model.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Items;

//	static
string Item ::
	get_class_name ()
{
	return "Item";
}

//  constructor
Item ::
	Item
	(
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass,
		bool new_mobile,
		bool new_solid,
		bool new_visible
	) :
	Object ("The name doesn't matter is this is an abstact base class."),
	size (new_size),
	mass (new_mass),
	mobile (new_mobile),
	solid (new_solid),
	visible (new_visible),
	mesh_name (new_mesh_name)
{
	Engines :: Log :: trace
	(
		me,
		Item :: get_class_name (),
		"",
		new_mesh_name,
		to_string (size),
		to_string (mass),
		bool_to_string (mobile),
		bool_to_string (solid),
		bool_to_string (visible)
	);
	assert (0 <= size . x);
	assert (0 <= size . y);
	assert (0 <= size . z);
	assert (0 <= mass);

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
	
	if (has_model ())
	{
		remove_model ();
	}
}

//	virtual
bool Item ::
	is_initialized ()
	const
{
	//	Engines :: Log :: trace (me, Item :: get_class_name (), "is_initialized");
	assert (Object :: is_initialized ());
	assert (0 <= size . x);
	assert (0 <= size . y);
	assert (0 <= size . z);
	assert (0 <= mass);
	
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
	set_model (Reference <Model> new_model)
{
	Engines :: Log :: trace (me, Item :: get_class_name (), "set_model", new_model . get_name ());
	
	assert (Item :: is_initialized ());
	assert (! has_model ());
	assert (new_model . points_to_object ());
	assert (new_model -> is_initialized ());

	model = new_model;

	assert (has_model ());
	assert (is_initialized ());
}

bool Item ::
	has_model () const
{
	assert (Item :: is_initialized ());
	
	return model . points_to_object ();
}

void Item ::
	remove_model ()
{
	assert (Item :: is_initialized ());
	assert (has_model ());

	//	The body destructs the item, not the other way around.
	
	model . reset_pointee ();
}

Reference <Model> Item ::
	get_model () const
{
	assert (Item :: is_initialized ());
	assert (has_model ());
	
	return model;
}

float Item ::
	get_volume () const
{
	assert (is_initialized ());

	return size . x * size . y * size . z;
}
