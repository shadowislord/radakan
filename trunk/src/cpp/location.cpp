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
	Location (int new_maximal_size) :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Engines :: Log :: trace (this -> me, Location <T> :: get_class_name ());
	assert ((0 < new_maximal_size) || (new_maximal_size == Container <T> :: unlimited));

	string implementation_name = this -> me . get_name () + "'s implementation";

	if (new_maximal_size == 1)
	{
		implementation . reset_pointee (new Slot <T> (implementation_name));
	}
	else
	{
		implementation . reset_pointee (new Set <T> (implementation_name, new_maximal_size));
	}

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
#include "engines/game.hpp"
#include "gui.hpp"
#include "items/character.hpp"
#include "items/container_item.hpp"
#include "model.hpp"
#include "strategies/strategy.hpp"

template class Location <GUI>;
template class Location <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Location <Items :: Container_Item <Items :: Item> >;
template class Location <Items :: Item>;
template class Location <Model>;
template class Location <Strategies :: Strategy <Engines :: Game> >;
template class Location <Strategies :: Strategy <Items :: Character> >;
