#include "algorithm_state_machine.hpp"
#include "log.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: State_Machines;

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
	Engines :: Log :: trace <Algorithm_State_Machine <T> > (this -> me, "");
	
	//	Do nothing.

	assert (Algorithm_State_Machine <T> :: is_initialized ());
}

//  destructor
template <class T> Algorithm_State_Machine <T> ::
	~Algorithm_State_Machine ()
{
	Engines :: Log :: trace <Algorithm_State_Machine <T> > (this -> me, "~");
	assert (Algorithm_State_Machine <T> :: is_initialized ());

	State_Machine <Algorithms :: Algorithm <T> > :: unset_active_state ();
}

//	virtual
template <class T> bool Algorithm_State_Machine <T> ::
	is_initialized ()
	const
{
	assert (State_Machine <Algorithms :: Algorithm <T> > :: is_initialized ());
//	assert (Object :: is_type <T> ());

	return true;
}

//	virtual
template <class T> void Algorithm_State_Machine <T> ::
	run (const Object & message)
{
	assert (is_initialized ());
	assert (State_Machine <Algorithms :: Algorithm <T> > :: has_active_state ());

	Algorithms :: Algorithm <T> * previous_state = NULL;
	Algorithms :: Algorithm <T> * current_state
		= & State_Machine <Algorithms :: Algorithm <T> > :: get_active_state ();

	do
	{
		previous_state = current_state;
		current_state = & State_Machine <Algorithms :: Algorithm <T> > :: get_active_state ()
			. recursive_transit (get_owner (), message);
		set_active_state (* current_state);
	}
	while (previous_state != current_state);
}

//	virtual
template <class T> void Algorithm_State_Machine <T> ::
	set_active_state (Algorithms :: Algorithm <T> & new_state)
{
	assert (is_initialized ());
	
	if (State_Machine <Algorithms :: Algorithm <T> > :: has_active_state ())
	{
		Algorithms :: Algorithm <T> & old_state = State_Machine <Algorithms :: Algorithm <T> > :: get_active_state ();

		if (old_state == new_state)
		{
			return;
		}

		old_state . recursive_exit (get_owner ());
		State_Machine <Algorithms :: Algorithm <T> > :: unset_active_state ();
		history . push (& old_state);
	}
	
	State_Machine <Algorithms :: Algorithm <T> > :: set_active_state (new_state);
	new_state . recursive_enter (get_owner ());
}

template <class T> void Algorithm_State_Machine <T> ::
	recall_previous_state ()
{
	assert (is_initialized ());
	assert (! history . empty ());

	State_Machine <Algorithms :: Algorithm <T> > :: set_active_state (* history . top ());
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

template class Algorithm_State_Machine <Engines :: Game>;
template class Algorithm_State_Machine <Items :: NPC>;
