#include "container_item.hpp"
#include "log.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Items;

//	static
template <class T> string Container_Item <T> ::
	get_class_name ()
{
	return "Container_Item <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Container_Item <T> ::
	Container_Item
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
	Location <T> (new_maximal_size)
{
	//	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
template <class T> Container_Item <T> ::
	~Container_Item ()
{
	Engines :: Log :: trace (me, Container_Item :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
template <class T> bool Container_Item <T> ::
	is_initialized ()
	const
{
	assert (Item :: is_initialized ());
	assert (Location <T> :: is_initialized ());

	return true;
}

//	virtual
template <class T> float Container_Item <T> ::
	get_total_mass ()
	const
{
	assert (is_initialized ());
	
	float total_mass = mass;

	for (Reference <T> i = this -> get_child (); i . points_to_object ();
		i = this -> get_another_child ())
	{
		total_mass += i -> get_total_mass ();
	}

	return total_mass;
}

//	virtual
template <class T> bool Container_Item <T> ::
	add (Reference <T> item)
{
	Engines :: Log :: trace (me, Container_Item :: get_class_name (), "add", item -> name);
	assert (is_initialized ());
	assert (item -> is_initialized ());
	assert (! this -> is_sealed ());
	assert (item -> is_initialized ());
	
	float total_volume = 0;

	for (Reference <T> i = this -> get_child (); i . points_to_object ();
		i = this -> get_another_child ())
	{
		total_volume += i -> get_volume ();
	}
	
	if (total_volume + item -> get_volume () > get_volume ())
	{
		return false;
	}

	bool check = Location <T> :: add (item);
	assert (check);

	return true;
}

template class Container_Item <Container_Item <Item> >;
template class Container_Item <Item>;

