#include "container.hpp"
#include "log.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Items;

//	static
const string Container ::
	get_class_name ()
{
	return "Container";
}

//  constructor
Container ::
	Container
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass,
		bool new_mobile,
		bool new_solid,
		bool new_visible,
		int new_maximal_size
	) :
	Object (new_name),
	Item
	(
		new_mesh_name,
		new_size,
		new_mass,
		new_mobile,
		new_solid,
		new_visible
	),
	Location <Item> (new_maximal_size)
{
	//	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
Container ::
	~Container ()
{
	Engines :: Log :: trace (me, Container :: get_class_name (), "~");
	assert (is_initialized ());

	forget_dependencies ();
}

//	virtual
bool Container ::
	is_initialized ()
	const
{
	assert (Item :: is_initialized ());
	assert (Set <Item> :: is_initialized ());

	return true;
}

//	virtual
float Container ::
	get_total_mass ()
	const
{
	assert (is_initialized ());
	
	float total_mass = mass;

	for (Item * i = get_child (); i != NULL; i = get_another_child ())
	{
		total_mass += i -> get_total_mass ();
	}

	return total_mass;
}

//	virtual
bool Container ::
	add (Item & item)
{
	Engines :: Log :: trace (me, Container :: get_class_name (), "add", item);
	assert (is_initialized ());
	assert (item . is_initialized ());
	assert (! is_sealed ());
	assert (is_initialized ());
	
	float total_volume = 0;

	for (Item * i = get_child (); i != NULL; i = get_another_child ())
	{
		total_volume += i -> get_volume ();
	}
	
	if (total_volume + item . get_volume () > get_volume ())
	{
		return false;
	}

	bool check = Location <Item> :: add (item);
	assert (check);

	return true;
}
