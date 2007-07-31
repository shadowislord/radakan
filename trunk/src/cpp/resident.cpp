#include "log.hpp"
#include "resident.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Resident <T> ::
	get_class_name ()
{
	return "Resident <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Resident <T> ::
	Resident () :
	Object ("The name doesn't matter as this class is an abstact class."),
	location (NULL)
{
	Engines :: Log :: trace (me, Resident <T> :: get_class_name ());

	//	Do nothing.

	assert (Resident <T> :: is_initialized ());
}

//  destructor
template <class T> Resident <T> ::
	~Resident ()
{
	Engines :: Log :: trace (me, Resident <T> :: get_class_name (), "~");
	assert (Resident <T> :: is_initialized ());

	//	Do nothing.
}

//	virtual
template <class T> bool Resident <T> ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

template <class T> void Resident <T> ::
	enter (const Location <T> & new_location)
{
	Engines :: Log :: trace (me, Resident <T> :: get_class_name (), "enter", new_location);
	assert (Resident <T> :: is_initialized ());
	assert (location == NULL);

	location = & new_location;
}

template <class T> void Resident <T> ::
	leave (const Location <T> & old_location)
{
	Engines :: Log :: trace (me, Resident <T> :: get_class_name (), "leave", old_location);
	assert (Resident <T> :: is_initialized ());
	
	if (location == NULL)
	{
		Engines :: Log :: log (me) << "I'm currently nowhere." << endl;
	}
	else
	{
		Engines :: Log :: log (me) << "I'm currently in " << * location << "." << endl;
	}
	assert (location == & old_location);

	location = NULL;
}

//	to avert linking errors:
#include "strategy.hpp"
#include "gui.hpp"
#include "item.hpp"
#include "model.hpp"
#include "tile.hpp"

template class Resident <Strategies :: Strategy>;
template class Resident <GUI>;
template class Resident <Items :: Item>;
template class Resident <Model>;
template class Resident <Tile>;
