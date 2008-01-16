#include "engines/log.hpp"
#include "slot.hpp"
#include "state_machine.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> string State_Machine <T> ::
	get_class_name ()
{
	return "State_Machine <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> State_Machine <T> ::
	State_Machine () :
	Object ("Doesn't matter."),
	Slot <T> ("Doesn't matter."),
	history (new vector <string> ())
{
	Engines :: Log :: trace (this -> me, State_Machine <T> :: get_class_name ());
	
	//	Do nothing.

	assert (State_Machine <T> :: is_initialized ());
}

//  destructor
template <class T> State_Machine <T> ::
	~State_Machine ()
{
	Engines :: Log :: trace (this -> me, State_Machine <T> :: get_class_name (), "~");
	assert (State_Machine <T> :: is_initialized ());

	//	Do nothing.
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
}

//	virtual
template <class T> bool State_Machine <T> ::
	is_initialized ()
	const
{
	assert (Slot <T> :: is_initialized ());

	return true;
}

//	virtual
template <class T> void State_Machine <T> ::
	drop (Reference <T> dropped)
{
	Engines :: Log :: trace
		(this -> me, State_Machine <T> :: get_class_name (), "drop", dropped . get_name ());
	assert (Slot <T> :: is_initialized ());
	assert (dropped . points_to_object ());
	assert (dropped -> is_initialized ());
	
	Slot <T> :: drop (dropped);
	history -> push_back (dropped . get_name ());
	
	Engines :: Log :: log (this -> me) << dropped << " was dropped as active state." << endl;
}

template <class T> bool State_Machine <T> ::
	has_active_state () const
{
	//	Engines :: Log :: trace (me, State_Machine <T> :: get_class_name (), "has_active_state");
	assert (State_Machine <T> :: is_initialized ());

	return ! Slot <T> :: is_empty ();
}

template <class T> Reference <T> State_Machine <T> ::
	get_active_state () const
{
	assert (State_Machine <T> :: is_initialized ());
	assert (has_active_state ());

	return Slot <T> :: get_child ();
}

template <class T> void State_Machine <T> ::
	set_active_state (Reference <T> new_state)
{
	//	Engines :: Log :: trace (me, State_Machine <T> :: get_class_name (), "set_active_state", new_state, bool_to_string (old_state_stay));
	assert (State_Machine <T> :: is_initialized ());

	if (! contains (new_state))	//	This also works for NULL.
	{
		if (has_active_state ())
		{
			drop (get_active_state ());
		}

		bool check = Slot <T> :: add (new_state);
		assert (check);

		Engines :: Log :: log (this -> me)
			<< "The active state changed to " << new_state << "." << endl;
	}
}

template <class T> const boost :: shared_ptr <vector <string> > State_Machine <T> ::
	get_history ()
{
	assert (is_initialized ());

	return history;
}

//	to avert linking errors:
#include "engines/game.hpp"
#include "gui.hpp"
#include "strategies/actions/action.hpp"
#include "strategies/behaviors/behavior.hpp"
#include "strategies/game_modes/game_mode.hpp"
#include "tile.hpp"

template class State_Machine <Strategies :: Actions :: Action>;
template class State_Machine <Strategies :: Behaviors :: Behavior>;
template class State_Machine <Strategies :: Game_Modes :: Game_Mode>;
template class State_Machine <GUI>;
template class State_Machine <Tile>;
