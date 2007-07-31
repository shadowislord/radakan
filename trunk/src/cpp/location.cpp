#include "location.hpp"
#include "log.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Location <T> ::
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
	
	for (T * child = Set <T> :: get_child (); child != NULL; child = Set <T> :: get_child ())
	{
		Engines :: Log :: log (this -> me) << "Deleting child '" << * child << "'..." << endl;
		delete child;
	}
	Engines :: Log :: log (this -> me) << "All children were deleted." << endl;
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
	add (T & t)
{
	Engines :: Log :: trace (this -> me, Location <T> :: get_class_name (), "add", t);
	assert (Location <T> :: is_initialized ());

	bool result = Set <T> :: add (t);
	if (result)
	{
		t . enter (* this);
	}

	return result;
}

//	virtual
template <class T> void Location <T> ::
	drop (Object & t, bool stay)
{
	Engines :: Log :: trace (this -> me, Location <T> :: get_class_name (), "drop", t, bool_to_string (stay));
	assert (Location <T> :: is_initialized ());
	
	t . to_type <T> () . leave (* this);
	Set <T> :: drop (t, stay);
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
