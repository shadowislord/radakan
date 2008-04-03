#include "engines/log.hpp"
#include "location.hpp"
#include "resident.hpp"
#include "slot.hpp"
#include "set.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> string Location <T> ::
	get_class_name ()
{
	return "Location <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Location <T> ::
	Location (unsigned int new_maximal_size, bool new_weak_children) :
	Object ("The name doesn't matter."),
	Container <T> (new_maximal_size, new_weak_children),
	implementation
	(
		new_maximal_size == 1
		? (Reference <Container <T> >
			(new Slot <T> (this -> me . get_name (true) + "'s implementation",
				new_weak_children)))
		: (Reference <Container <T> >
			(new Set <T> (this -> me . get_name (true) + "'s implementation",
				new_maximal_size, new_weak_children)))
	)
{
	Engines :: Log :: trace (this -> me, Location <T> :: get_class_name ());
	assert (0 < new_maximal_size);

	assert (Location <T> :: is_initialized ());
}

//  destructor
template <class T> Location <T> ::
	~Location ()
{
	Engines :: Log :: trace (this -> me, Location <T> :: get_class_name (), "~");
	assert (Location <T> :: is_initialized ());
	
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
}

//	virtual
template <class T> bool Location <T> ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	assert (implementation . points_to_object ());

	return true;
}

template <class T> bool Location <T> ::
	is_empty () const
{
	assert (Location <T> :: is_initialized ());

	return implementation -> is_empty ();
}

//	virtual
template <class T> bool Location <T> ::
	contains (const Reference <T> & contained)
	const
{
//	Engines :: Log :: trace (this -> me, Location <T> :: get_class_name (), "contains", contained . get_name ());
	assert (Location <T> :: is_initialized ());

	return implementation -> contains (contained);
}

//	virtual
template <class T> bool Location <T> ::
	add (Reference <T> additive)
{
	Engines :: Log :: trace (this -> me, Location <T> :: get_class_name (), "add", additive . get_name ());
	assert (Location <T> :: is_initialized ());
	assert (additive . points_to_object ());
	assert (additive -> is_initialized ());

	bool result = implementation -> add (additive);
	if (result)
	{
		additive -> Resident <T> :: enter (Reference <Location <T> > (this));
	}

	return result;
}

//	virtual
template <class T> void Location <T> ::
	drop (Reference <T> dropped)
{
	Engines :: Log :: trace (this -> me, Location <T> :: get_class_name (), "drop", dropped . get_name ());
	assert (Location <T> :: is_initialized ());
	assert (dropped . points_to_object ());
	assert (dropped -> is_initialized ());

	dropped -> Resident <T> :: leave ();
	return implementation -> drop (dropped);
}

template <class T> Reference <T> Location <T> ::
	get_child ()
	const
{
//	Engines :: Log :: trace (this -> me, Location <T> :: get_class_name (), get_child);
	assert (Location <T> :: is_initialized ());

	return implementation -> get_child ();
}

template <class T> Reference <T> Location <T> ::
	get_another_child ()
	const
{
//	Engines :: Log :: trace (this -> me, Location <T> :: get_class_name (), get_another_child);
	assert (Location <T> :: is_initialized ());

 return implementation -> get_another_child ();
}

//	to avert linking errors:
#include "items/container_item.hpp"

template class Location <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Location <Items :: Container_Item <Items :: Item> >;
template class Location <Items :: Item>;
