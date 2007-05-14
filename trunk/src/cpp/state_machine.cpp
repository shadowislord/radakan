#include "state_machine.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string State_Machine <T> ::
	class_name ("State_Machine <" + T :: class_name + ">");

//  constructor
template <class T> State_Machine <T> ::
	State_Machine () :
	Object ("The name doesn't matter as this class is an abstact class."),
	active_state (NULL)
{
	log (debugging) << class_name << " ()" << endl;
	assert (Object :: is_initialized ());

	assert (State_Machine <T> :: is_initialized ());
}

//  destructor
template <class T> State_Machine <T> ::
	~State_Machine ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
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
	assert (is_initialized ());

	return (active_state != NULL);
}

//	virtual
template <class T> T & State_Machine <T> ::
	get_active_state () const
{
	assert (is_initialized ());
	assert (has_active_state ());

	return * active_state;
}

//	virtual
template <class T> void State_Machine <T> ::
	set_active_state (T & t)
{
//	Object :: log (debugging) << "set_active_state (" << t << ")" << endl;
	assert (State_Machine <T> :: is_initialized ());

	if (active_state != & t)
	{
		string active_state_name = "NULL";
		if (active_state != NULL)
		{
			active_state_name =  * active_state;
		}
		
		active_state = & t;
		
		log (debugging) << "The active state changed from " << active_state_name << " to " << t << "." << endl;
	}
}

//	virtual
template <class T> void State_Machine <T> ::
	unset_active_state ()
{
	log (debugging) << "unset_active_state ()" << endl;
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
