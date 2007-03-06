#include "container.hpp"

using namespace std;
using namespace tsl;

//  constructor
Container ::
	Container
	(
		string new_name,
		string new_mesh_name,
		float new_volume,
		float new_mass,
		bool new_mobile,
		bool new_solid,
		bool new_visible
	) :
	Object (new_name),
	Item
	(
		new_mesh_name,
		new_volume,
		new_mass,
		new_mobile,
		new_solid,
		new_visible
	),
	sealed (false)
{
	assert (Item :: is_initialized () && Disjoint_Set <Item> :: is_initialized ());
	
	assert (is_initialized ());
}

//  destructor
Container ::
	~Container ()
{
	log (TSL_DEBUG) << "~" << get_class_name () << " ()" << endl;
	assert (is_initialized ());
}

//	virtual
bool Container ::
	is_initialized ()
	const
{
	return Item :: is_initialized () && Disjoint_Set <Item> :: is_initialized ();
}

//	static
string Container ::
	get_class_name ()
{
	return "Container";
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
	log (TSL_DEBUG) << "add (" << item << ")" << endl;
	assert (is_initialized ());
	assert (item . is_initialized ());
	assert (! sealed);
	
	assert (is_initialized ());
	
	float total_volume = 0;

	for (Item * i = get_child (); i != NULL; i = get_another_child ())
	{
		total_volume += i -> volume;
	}
	
	if (total_volume + item . volume > volume)
	{
		return false;
	}

	bool check = Disjoint_Set <Item> :: add (item);
	assert (check);

	return true;
}

void Container ::
	seal ()
{
	sealed = true;
}

//	static
Item & Container ::
	create
	(
		string new_name,
		string new_mesh_name,
		float new_volume,
		float new_mass,
		bool new_mobile,
		bool new_solid,
		bool new_visible
	)
{
	Item * temp =
		new Container
		(
			new_name,
			new_mesh_name,
			new_volume,
			new_mass,
			new_mobile,
			new_solid,
			new_visible
		);

	return * temp;
}
