#include "engines/log.hpp"
#include "resident.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> string Resident <T> ::
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
	enter (const Reference <Location <T> > & new_location)
{
	Engines :: Log :: trace (me, Resident <T> :: get_class_name (), "enter", new_location . get_name ());
	assert (Resident <T> :: is_initialized ());
	assert (! location . points_to_object ());
	assert (new_location . points_to_object ());

	location = new_location;
}

template <class T> void Resident <T> ::
	leave ()
{
	Engines :: Log :: trace (me, Resident <T> :: get_class_name (), "leave");
	assert (Resident <T> :: is_initialized ());
	assert (location . points_to_object ());

	Engines :: Log :: log (me) << "I'm leaving " << location << "." << endl;

	location . reset_pointee ();
}

//	to avert linking errors:
#include "engines/game.hpp"
#include "items/characters/character.hpp"
#include "items/container_item.hpp"
#include "gui.hpp"
#include "model.hpp"
#include "strategies/actions/action.hpp"
#include "strategies/behaviors/behavior.hpp"
#include "strategies/game_modes/game_mode.hpp"

template class Resident <GUI>;
template class Resident <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Resident <Items :: Container_Item <Items :: Item> >;
template class Resident <Items :: Item>;
template class Resident <Model>;
template class Resident <Strategies :: Actions :: Action>;
template class Resident <Strategies :: Behaviors :: Behavior>;
template class Resident <Strategies :: Game_Modes :: Game_Mode>;

