#include "container.hpp"

using namespace std;
using namespace tsl;

//  constructor
Container ::
	Container
	(
		string new_name,
		string mesh_name,
		float new_volume,
		float new_weight,
		bool new_mobile,
		bool new_solid,
		bool new_visible
	) :
	Object (new_name),
	Item
	(
		new_name,
		mesh_name,
		new_volume,
		new_weight,
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
	trace () << "~" << get_class_name () << " ()" << endl;
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
	get_total_weight ()
	const
{
	assert (is_initialized ());
	
	float total_weight = weight;

	for (Item * i = get_child (); i != NULL; i = get_another_child ())
	{
		total_weight += i -> get_total_weight ();
	}

	return total_weight;
}

//	virtual
bool Container ::
	add (Item & item)
{
	trace () << "add (" << item << ")" << endl;
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
		float new_weight,
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
			new_weight,
			new_mobile,
			new_solid,
			new_visible
		);

	return * temp;
}
