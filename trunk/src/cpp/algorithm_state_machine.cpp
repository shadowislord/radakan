#include "algorithm_state_machine.hpp"
#include "log.hpp"
#include "speech_state.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Algorithm_State_Machine <T> ::
	get_class_name ()
{
	return "Algorithm_State_Machine <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Algorithm_State_Machine <T> ::
	Algorithm_State_Machine () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Log :: trace <Algorithm_State_Machine <T> > (this -> me, "");
	assert (State_Machine <Algorithm <T> > :: is_initialized ());

	assert (Algorithm_State_Machine <T> :: is_initialized ());
}

//  destructor
template <class T> Algorithm_State_Machine <T> ::
	~Algorithm_State_Machine ()
{
	Log :: trace <Algorithm_State_Machine <T> > (this -> me, "~");
	assert (Algorithm_State_Machine <T> :: is_initialized ());

	State_Machine <Algorithm <T> > :: unset_active_state ();
}

//	virtual
template <class T> bool Algorithm_State_Machine <T> ::
	is_initialized ()
	const
{
	assert (State_Machine <Algorithm <T> > :: is_initialized ());
//	assert (Object :: is_type <T> ());

	return true;
}

//	virtual
template <class T> void Algorithm_State_Machine <T> ::
	run ()
{
	assert (is_initialized ());
	assert (State_Machine <Algorithm <T> > :: has_active_state ());

	set_active_state
	(
		State_Machine <Algorithm <T> > :: get_active_state ()
			. full_transit (get_owner ())
	);
}

//	virtual
template <class T> void Algorithm_State_Machine <T> ::
	set_active_state (Algorithm <T> & new_state)
{
	if (State_Machine <Algorithm <T> > :: has_active_state ())
	{
		Algorithm <T> & old_state = State_Machine <Algorithm <T> > :: get_active_state ();

		if (old_state == new_state)
		{
			return;
		}
	
		old_state . exit (get_owner ());
		State_Machine <Algorithm <T> > :: unset_active_state ();
		if (old_state . Object :: is_type <Speech_State> ())
		{
			delete & old_state;
		}
		else
		{
			history . push (& old_state);
		}
	}
	
	State_Machine <Algorithm <T> > :: set_active_state (new_state);
	new_state . enter (get_owner ());
}

template <class T> void Algorithm_State_Machine <T> ::
	recall_previous_state ()
{
	assert (is_initialized ());
	assert (! history . empty ());

	State_Machine <Algorithm <T> > :: set_active_state (* history . top ());
	history . pop ();
}

template <class T> T & Algorithm_State_Machine <T> ::
	get_owner ()
	const
{
	assert (is_initialized ());

	return Object :: to_type <T> ();
}

//	to avert linking errors:
#include "game.hpp"
#include "npc.hpp"

template class Algorithm_State_Machine <Game>;
template class Algorithm_State_Machine <NPC>;
