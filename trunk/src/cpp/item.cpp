#include "item.hpp"
#include "log.hpp"
#include "model.hpp"
#include "world.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Items;

//	static
const string Item ::
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
	entity (* World :: get () . getSceneManager () -> createEntity
										(name + "'s entity", new_mesh_name)),
	model (NULL)
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
	set_model (Model & new_model)
{
	//	Engines :: Log :: trace (me, Item :: get_class_name (), "set_model", new_model);
	assert (Item :: is_initialized ());
	assert (! has_model ());

	model = & new_model;

	assert (is_initialized ());
}

bool Item ::
	has_model () const
{
	assert (Item :: is_initialized ());
	
	return (model != NULL);
}

void Item ::
	remove_model ()
{
	assert (Item :: is_initialized ());
	assert (has_model ());

	//	The body destructs the item, not the other way around.
	
	model = NULL;
}

Model & Item ::
	get_model () const
{
	assert (Item :: is_initialized ());
	assert (has_model ());
	
	return * model;
}

//	virtual
OgreOde :: Geometry & Item ::
	create_geometry ()
{
	assert (is_initialized ());
	assert (! has_model ());

	OgreOde :: Geometry * geometry = new OgreOde :: BoxGeometry (size, & World :: get (), World :: get () . getDefaultSpace ());

	if (mobile)
	{
		OgreOde :: Body * body = new OgreOde :: Body (& World :: get (), name);
		body -> setMass (OgreOde :: BoxMass (mass, size));
		geometry -> setBody (body);
	}

	return * geometry;
}

float Item ::
	get_volume () const
{
	assert (is_initialized ());

	return size . x * size . y * size .z;
}
