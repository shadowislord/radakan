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
	active_state (NULL)
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

	if (active_state != NULL)
	{
		assert (contains (* active_state));
	}

	return true;
}

//	virtual
template <class T> void State_Machine <T> ::
	drop (Object & t, bool stay)
{
	Engines :: Log :: trace (this -> me, State_Machine <T> :: get_class_name (), "drop", t, bool_to_string (stay));
	assert (Location <T> :: is_initialized ());
	
	if (& t != active_state)	//	This also works for NULL.
	{
		unset_active_state ();
	}
	Location <T> :: drop (t, stay);
}

//	virtual
template <class T> bool State_Machine <T> ::
	has_active_state () const
{
	//	Engines :: Log :: trace (me, State_Machine <T> :: get_class_name (), "has_active_state");
	assert (State_Machine <T> :: is_initialized ());

	return (active_state != NULL);
}

//	virtual
template <class T> T & State_Machine <T> ::
	get_active_state () const
{
	assert (State_Machine <T> :: is_initialized ());
	assert (has_active_state ());

	return * active_state;
}

//	virtual
template <class T> void State_Machine <T> ::
	set_active_state (T & new_state)
{
	Engines :: Log :: trace (me, State_Machine <T> :: get_class_name (), "set_active_state", new_state);
	assert (State_Machine <T> :: is_initialized ());

	if (& new_state != active_state)	//	This also works for NULL.
	{
		//	If this doesn't succeed, that meant I already have 'new_state' as a Resident,
		//	which is fine too.
		Location <T> :: add (new_state);

		if (has_active_state ())
		{
			unset_active_state ();
		}

		active_state = & new_state;
		Engines :: Log :: log (me) << "The active state changed to " << new_state << "." << endl;
	}
}

//	virtual
template <class T> void State_Machine <T> ::
	unset_active_state ()
{
	Engines :: Log :: trace (me, State_Machine <T> :: get_class_name (), "unset_active_state");
	assert (State_Machine <T> :: is_initialized ());
	assert (has_active_state ());

	history . push_back (* active_state);
	
	Engines :: Log :: log (me) << * active_state << " was dropped as active state." << endl;
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
