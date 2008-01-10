#include "engines/log.hpp"
#include "items/container_item.hpp"
#include "pointer.hpp"

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
		float new_mass,
		Ogre :: Vector3 new_size,
		const Reference <Mesh_Data> new_mesh_data,
		int new_maximal_size
	) :
	Object (new_name),
	Item
	(
		new_mass,
		new_size,
		new_mesh_data
	),
	Location <T> (new_maximal_size),
	filled_volume (0),
	total_mass (new_mass)
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
	assert (0 <= filled_volume);
	assert (filled_volume <= get_volume ());
	assert (0 <= total_mass);

	return true;
}

//	virtual
template <class T> float Container_Item <T> ::
	get_total_mass ()
	const
{
	assert (is_initialized ());
	
	return total_mass;
}

//	virtual
template <class T> bool Container_Item <T> ::
	add (Reference <T> item)
{
	Engines :: Log :: trace (me, Container_Item :: get_class_name (), "add", item . get_name ());
	assert (is_initialized ());
	assert (! this -> is_sealed ());
	assert (item -> is_initialized ());
	
	if (filled_volume + item -> get_volume () > get_volume ())
	{
		return false;
	}

	bool check = Location <T> :: add (item);
	assert (check);

	filled_volume += item -> get_volume ();
	total_mass += item -> get_total_mass ();

	return true;
}

//	virtual
template <class T> void Container_Item <T> ::
	drop (Reference <T> item)
{
	Engines :: Log :: trace (me, Container_Item :: get_class_name (), "drop", item . get_name ());
	assert (is_initialized ());
	assert (item -> is_initialized ());
	assert (contains (item));
	
	filled_volume -= item -> get_volume ();
	total_mass -= item -> get_total_mass ();

	Location <T> :: drop (item);
}

template class Container_Item <Container_Item <Item> >;
template class Container_Item <Item>;
