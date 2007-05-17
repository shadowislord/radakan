#include "container.hpp"
#include "log.hpp"

using namespace std;
using namespace TSL;

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
	Set <Item> ("", new_maximal_size)
{
	assert (Item :: is_initialized () && Set <Item> :: is_initialized ());
	
	assert (is_initialized ());
}

//  destructor
Container ::
	~Container ()
{
	Log :: trace <Container> (me, "~");
	assert (is_initialized ());
}

//	virtual
bool Container ::
	is_initialized ()
	const
{
	return Item :: is_initialized () && Set <Item> :: is_initialized ();
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
	Log :: trace <Container> (me, "add", item);
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

	bool check = Set <Item> :: add (item);
	assert (check);

	return true;
}

//	static
Item & Container ::
	create
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass,
		bool new_mobile,
		bool new_solid,
		bool new_visible,
		int new_maximal_size
	)
{
	Item * temp =
		new Container
		(
			new_name,
			new_mesh_name,
			new_size,
			new_mass,
			new_mobile,
			new_solid,
			new_visible,
			new_maximal_size
		);

	return * temp;
}
