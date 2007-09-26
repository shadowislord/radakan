#include "log.hpp"
#include "resident.hpp"

using namespace std;
using namespace Radakan;

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
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
}

//	virtual
template <class T> bool Resident <T> ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

template <class T> void Resident <T> ::
	enter (Reference <const Location <T> > new_location)
{
	Engines :: Log :: trace (me, Resident <T> :: get_class_name (), "enter", new_location -> name);
	assert (Resident <T> :: is_initialized ());
	assert (! location . points_to_object ());

	location = new_location;
}

template <class T> void Resident <T> ::
	leave ()
{
	Engines :: Log :: trace (me, Resident <T> :: get_class_name (), "leave");
	assert (Resident <T> :: is_initialized ());

	Engines :: Log :: log (me) << "I'm leaving " << location -> name << "." << endl;

	location . reset_pointee ();
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
