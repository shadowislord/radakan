#include "location.hpp"
#include "log.hpp"

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
	Object ("The name doesn't matter as this class is an abstact class."),
	Set <T> ("The name doesn't matter as this class is an abstact class.", new_maximal_size)
{
	Engines :: Log :: trace (this -> me, Location <T> :: get_class_name ());
	
	//	Do nothing.

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
	assert (Set <T> :: is_initialized ());

	return true;
}

//	virtual
template <class T> bool Location <T> ::
	add (Reference <T> additive)
{
	Engines :: Log :: trace (this -> me, Location <T> :: get_class_name (), "add", additive -> name);
	assert (Location <T> :: is_initialized ());

	bool result = Set <T> :: add (additive);
	if (result)
	{
		additive -> enter (Reference <const Location <T> > (this));
	}

	return result;
}

//	virtual
template <class T> void Location <T> ::
	drop (Reference <T> dropped)
{
	Engines :: Log :: trace (this -> me, Location <T> :: get_class_name (), "drop", dropped -> name);
	assert (Location <T> :: is_initialized ());
	
	dropped -> leave ();
	Set <T> :: drop (dropped);
}

//	to avert linking errors:
#include "strategy.hpp"
#include "gui.hpp"
#include "item.hpp"
#include "model.hpp"
#include "tile.hpp"

template class Location <Strategies :: Strategy>;
template class Location <GUI>;
template class Location <Items :: Item>;
template class Location <Model>;
template class Location <Tile>;
