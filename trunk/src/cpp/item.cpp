#include "item.hpp"
#include "body.hpp"

using namespace std;
using namespace tsl;

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
	entity (* Environment :: get () . getSceneManager () -> createEntity
										(* this + "'s entity", new_mesh_name)),
	body (NULL)
{
	log (debugging) << get_class_name () << " (" << string :: data () << ", "
		<< to_string (size) << ", " << mass << ", " << bool_to_string (mobile) << ", "
		<< bool_to_string (solid) << ", " << bool_to_string (visible) << ")"
		<< endl;
	assert (Object :: is_initialized ());
	assert (Environment :: is_instantiated ());
	assert (Environment :: get () . is_initialized ());
	assert (0 <= size . x);
	assert (0 <= size . y);
	assert (0 <= size . z);
	assert (0 <= mass);

	assert (Item :: is_initialized ());
}

//  destructor
Item ::
	~Item ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;
	assert (Item :: is_initialized ());

	if (has_body ())
	{
		remove_body ();
	}
	
	assert (Object :: is_initialized ());
}

//	virtual
bool Item ::
	is_initialized ()
	const
{
//	log (debugging) << get_class_name () << " :: is_initialized ()" << endl;
	assert (Object :: is_initialized ());
	assert (Environment :: is_instantiated ());
	assert (Environment :: get () . is_initialized ());
	assert (0 <= size . x);
	assert (0 <= size . y);
	assert (0 <= size . z);
	assert (0 <= mass);
	
	return true;
}

//	static
string Item ::
	get_class_name ()
{
	return "Item";
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
	set_body (Body & new_body)
{
	//	log (debugging) << "set_body ()" << endl;
	assert (Item :: is_initialized ());
	assert (! has_body ());

	body = & new_body;

	assert (is_initialized ());
}

bool Item ::
	has_body () const
{
	assert (Item :: is_initialized ());
	
	return (body != NULL);
}

void Item ::
	remove_body ()
{
	assert (Item :: is_initialized ());
	assert (has_body ());
	
	body = NULL;
}

Body & Item ::
	get_body () const
{
	assert (Item :: is_initialized ());
	assert (has_body ());
	
	return * body;
}

//	virtual
OgreOde :: Geometry & Item ::
	create_geometry ()
{
	assert (is_initialized ());
	assert (! has_body ());

	OgreOde :: Geometry * geometry = new OgreOde :: BoxGeometry (size, & Environment :: get (), Environment :: get () . getDefaultSpace ());

	if (mobile)
	{
		OgreOde :: Body * body = new OgreOde :: Body (& Environment :: get (), string :: data ());
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
