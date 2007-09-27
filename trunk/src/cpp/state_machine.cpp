#include "log.hpp"
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
	Location <T> (1)
{
	Engines :: Log :: trace (me, State_Machine <T> :: get_class_name ());
	
	//	Do nothing.

	assert (State_Machine <T> :: is_initialized ());
}

//  destructor
template <class T> State_Machine <T> ::
	~State_Machine ()
{
	Engines :: Log :: trace (me, State_Machine <T> :: get_class_name (), "~");
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
	assert (Location <T> :: is_initialized ());

	return true;
}

//	virtual
template <class T> void State_Machine <T> ::
	drop (Reference <T> dropped)
{
	Engines :: Log :: trace (this -> me, State_Machine <T> :: get_class_name (), "drop", dropped -> name);
	assert (Location <T> :: is_initialized ());
	assert (dropped -> is_initialized ());
	
	Engines :: Log :: log (me) << dropped -> name << " was dropped as active state." << endl;
	
	Location <T> :: drop (dropped);
	history -> push_back (dropped -> name);
}

template <class T> bool State_Machine <T> ::
	has_active_state () const
{
	//	Engines :: Log :: trace (me, State_Machine <T> :: get_class_name (), "has_active_state");
	assert (State_Machine <T> :: is_initialized ());

	return Location <T> :: get_child () . points_to_object ();
}

template <class T> Reference <T> State_Machine <T> ::
	get_active_state () const
{
	assert (State_Machine <T> :: is_initialized ());
	assert (has_active_state ());

	return Location <T> :: get_child ();
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

		bool check = Location <T> :: add (new_state);
		assert (check);

		Engines :: Log :: log (me) << "The active state changed to " << new_state -> name << "." << endl;
	}
}

template <class T> const boost :: shared_ptr <vector <string> > State_Machine <T> ::
	get_history ()
{
	assert (is_initialized ());

	return history;
}

//	to avert linking errors:
#include "gui.hpp"
#include "strategy.hpp"
#include "tile.hpp"

template class State_Machine <Strategies :: Strategy>;
template class State_Machine <GUI>;
template class State_Machine <Tile>;
