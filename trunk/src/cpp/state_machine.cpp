#include "log.hpp"
#include "state_machine.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string State_Machine <T> ::
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
	drop (Object & t, bool stay)
{
	Engines :: Log :: trace (this -> me, State_Machine <T> :: get_class_name (), "drop", t, bool_to_string (stay));
	assert (Location <T> :: is_initialized ());
	assert (t . is_type <T> ());
	
	Engines :: Log :: log (me) << t << " was dropped as active state." << endl;
	
	history . push_back (t);
	Location <T> :: drop (t, stay);
}

template <class T> bool State_Machine <T> ::
	has_active_state () const
{
	//	Engines :: Log :: trace (me, State_Machine <T> :: get_class_name (), "has_active_state");
	assert (State_Machine <T> :: is_initialized ());

	return (Location <T> :: get_child () != NULL);
}

template <class T> T & State_Machine <T> ::
	get_active_state () const
{
	assert (State_Machine <T> :: is_initialized ());
	assert (has_active_state ());

	return * Location <T> :: get_child ();
}

template <class T> void State_Machine <T> ::
	set_active_state (T & new_state, bool old_state_stay)
{
	Engines :: Log :: trace (me, State_Machine <T> :: get_class_name (), "set_active_state", new_state, bool_to_string (old_state_stay));
	assert (State_Machine <T> :: is_initialized ());

	if (! contains (new_state))	//	This also works for NULL.
	{
		if (has_active_state ())
		{
			drop (get_active_state (), old_state_stay);
		}

		bool check = Location <T> :: add (new_state);
		assert (check);

		Engines :: Log :: log (me) << "The active state changed to " << new_state << "." << endl;
	}
}

template <class T> const vector <string> & State_Machine <T> ::
	get_history ()
{
	assert (is_initialized ());

	return history;
}

//	to avert linking errors:
#include "gui.hpp"
#include "algorithm.hpp"
#include "tile.hpp"

template class State_Machine <Algorithms :: Algorithm>;
template class State_Machine <GUI>;
template class State_Machine <Tile>;
