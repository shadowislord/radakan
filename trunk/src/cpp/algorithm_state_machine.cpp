#include "algorithm_state_machine.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Algorithm_State_Machine <T> ::
	class_name ("Algorithm_State_Machine <" + T :: class_name + ">");

//  constructor
template <class T> Algorithm_State_Machine <T> ::
	Algorithm_State_Machine () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Object :: log (Object :: debugging) << class_name << " ()" << endl;
	assert (State_Machine <Algorithm <T> > :: is_initialized ());

	assert (Algorithm_State_Machine <T> :: is_initialized ());
}

//  destructor
template <class T> Algorithm_State_Machine <T> ::
	~Algorithm_State_Machine ()
{
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

	Algorithm <T> & old_state = State_Machine <Algorithm <T> > :: get_active_state ();

	T & owner = Object :: to_type <T> (); 

	Algorithm <T> & new_state = old_state . full_transit (owner);
	
	if (old_state != new_state)
	{
		old_state . exit (owner);
		history . push (& old_state);
		State_Machine <Algorithm <T> > :: set_active_state (new_state);
		new_state . enter (owner);
	}
}

template <class T> void Algorithm_State_Machine <T> ::
	recall_previous_state ()
{
	assert (is_initialized ());
	assert (! history . empty ());

	State_Machine <Algorithm <T> > :: set_active_state (* history . top ());
	history . pop ();
}

//	to avert linking errors:
#include "game.hpp"
#include "npc.hpp"

template class Algorithm_State_Machine <Game>;
template class Algorithm_State_Machine <NPC>;
