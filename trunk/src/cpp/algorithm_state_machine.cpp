#include "algorithm_state_machine.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <class T> Algorithm_State_Machine <T> ::
	Algorithm_State_Machine () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Object :: log (Object :: debugging) << get_class_name () << " ()" << endl;
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

//	static
template <class T> string Algorithm_State_Machine <T> ::
	get_class_name ()
{
	return "Algorithm_State_Machine <" + T :: get_class_name () + ">";
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
		if (history . empty () || (history . top () != & old_state))
		{
			history . push (& old_state);
		}
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
#include "tsl.hpp"
#include "npc.hpp"

template class Algorithm_State_Machine <NPC>;
template class Algorithm_State_Machine <TSL>;
