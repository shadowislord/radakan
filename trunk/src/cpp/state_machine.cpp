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
	Object ("The name doesn't matter as this class is an abstact class."),
	active_state (NULL)
{
	Log :: trace <State_Machine <T> > (me);
	assert (Object :: is_initialized ());

	assert (State_Machine <T> :: is_initialized ());
}

//  destructor
template <class T> State_Machine <T> ::
	~State_Machine ()
{
	Log :: trace <State_Machine <T> > (me, "~");
	assert (State_Machine <T> :: is_initialized ());
}

//	virtual
template <class T> bool State_Machine <T> ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	assert ((active_state == NULL) || active_state -> is_initialized ());

	return true;
}

//	virtual
template <class T> bool State_Machine <T> ::
	has_active_state () const
{
	//	Log :: trace <State_Machine <T> > (me, "has_active_state");
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
	Log :: trace <State_Machine <T> > (me, "set_active_state", new_state);
	assert (State_Machine <T> :: is_initialized ());

	if (active_state != & new_state)	//	This also works for NULL.
	{
		string active_state_name = "NULL";
		if (active_state != NULL)
		{
			active_state_name =  * active_state;
		}
		
		active_state = & new_state;
		
		Log :: log (me) << "The active state changed from " << active_state_name << " to " << new_state << "." << endl;
	}
}

//	virtual
template <class T> void State_Machine <T> ::
	unset_active_state ()
{
	Log :: trace <State_Machine <T> > (me, "unset_active_state");
	assert (State_Machine <T> :: is_initialized ());

	active_state = NULL;
}

//	to avert linking errors:
#include "game.hpp"
#include "gui.hpp"
#include "npc.hpp"
#include "tile.hpp"

template class State_Machine <Algorithm <Game> >;
template class State_Machine <Algorithm <NPC> >;
template class State_Machine <GUI>;
template class State_Machine <Tile>;
