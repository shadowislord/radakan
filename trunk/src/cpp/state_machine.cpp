#include "state_machine.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <class T> State_Machine <T> ::
	State_Machine () :
	Object ("The name doesn't matter as this class is an abstact class."),
	active_state (NULL)
{
	Object :: trace () << get_class_name () << " ()" << endl;
	assert (Object :: is_initialized ());

	assert (State_Machine <T> :: is_initialized ());
}

//  destructor
template <class T> State_Machine <T> ::
	~State_Machine ()
{
	Object :: trace () << "~" << get_class_name () << " ()" << endl;
	assert (State_Machine <T> :: is_initialized ());
}

//	virtual
template <class T> bool State_Machine <T> ::
	is_initialized ()
	const
{
	assert (Object :: warn <State_Machine <T> > (Object :: is_initialized ()));
	assert
	(
		Object :: warn <State_Machine <T> >
		(
			(active_state == NULL)
									|| active_state -> is_initialized ()
		)
	);

	return true;
}

//	static
template <class T> string State_Machine <T> ::
	get_class_name ()
{
	return "State_Machine <" + T :: get_class_name () + ">";
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
//	Object :: trace () << "set_active_state (" << t << ")" << endl;
	assert (State_Machine <T> :: is_initialized ());

	if (active_state != & t)
	{
		string active_state_name = "NULL";
		if (active_state != NULL)
		{
			active_state_name =  * active_state;
		}
		
		active_state = & t;
		
		debug () << "The active state changed from " << active_state_name << " to " << t << "." << endl;
	}
}

//	virtual
template <class T> void State_Machine <T> ::
	unset_active_state ()
{
	Object :: trace () << "unset_active_state ()" << endl;
	assert (State_Machine <T> :: is_initialized ());

	active_state = NULL;
}

//	to avert linking errors:
#include "gui.hpp"
#include "npc.hpp"
#include "tile.hpp"
#include "tsl.hpp"

template class State_Machine <Algorithm <NPC> >;
template class State_Machine <Algorithm <TSL> >;
template class State_Machine <GUI>;
template class State_Machine <Tile>;
