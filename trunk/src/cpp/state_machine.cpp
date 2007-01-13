#include "state_machine.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <typename T> State_Machine <T> ::
	State_Machine (T * new_owner) :
	Object (* new_owner + "'s state machine"),
	State <T> (* new_owner),
	Set <State <T> > (* new_owner + "'s state machine")
{
	Object :: trace () << "State_Machine ()" << endl;
	assert (Set <State <T> > :: is_initialized ());

	active_child_state = NULL;
	
	assert (State_Machine <T> :: is_initialized ());
}

//  destructor
template <typename T> State_Machine <T> ::
	~State_Machine ()
{
	assert (State_Machine :: is_initialized ());
}

//	virtual
template <typename T> bool State_Machine <T> ::
	is_initialized ()
	const
{
//	trace () << "State_Machine :: is_initialized () A" << endl;
	assert (Object :: warn <State_Machine <T> > (State <T> :: is_initialized ()));
//	trace () << "State_Machine :: is_initialized () B" << endl;
	assert (Object :: warn <State_Machine <T> > (Set <State <T> > :: is_initialized ()));
//	trace () << "State_Machine :: is_initialized () C" << endl;

	return (Object :: warn <State_Machine <T> > ((active_child_state == NULL) || active_child_state -> is_initialized ()));
}

//	static
template <typename T> string State_Machine <T> ::
	get_type_name ()
{
	return "state machine";
}

//	virtual
template <typename T> string State_Machine <T> ::
	think ()
{
	assert (is_initialized ());
	assert (active_child_state != NULL);
	assert (! State <T> :: owner . is_dead ());
	
	return active_child_state -> think ();
}

//	virtual
template <typename T> bool State_Machine <T> ::
	add (State <T> & state)
{
	assert (is_initialized ());
	assert (state . is_initialized ());

	bool result = Set <State <T> > :: add (state);

	assert (result);

	active_child_state = & state;

	return true;
}

//virtual
template <typename T> template <typename U> void State_Machine <T> ::
	change_active_state ()
{
	Object :: trace () << "change_active_state <" << U :: get_type_name () << "> ()" << endl;
	assert (Object :: is_initialized ());

	//	!!! doesn't work yet
	//	active_child_state = Set <State <T> > :: get_child <U> ();

	if (active_child_state == NULL)
	{
		add <U> ();
		//	!!! doesn't work yet
		//	active_child_state = Set <State <T> > :: get_child <U> ();
	}

	assert (active_child_state != NULL);
}

template <typename T> template <typename U> void State_Machine <T> ::
	add ()
{
	Object :: trace () << "add <" << U :: get_type_name () << "> ()" << endl;
	assert (is_initialized ());

	//	!!! doesn't work yet
	//	assert (Set <State <T> > :: get_child <U> () == NULL);

	add (* (new U (State <T> :: owner)) );
}

#include "dead_state.hpp"
#include "fight_state.hpp"
#include "peace_state.hpp"

template class State_Machine <Character>;

template void State_Machine <Character> :: change_active_state <Dead_State> ();
template void State_Machine <Character> :: change_active_state <Fight_State> ();
template void State_Machine <Character> :: change_active_state <Peace_State> ();

template void State_Machine <Character> :: add <Dead_State> ();
template void State_Machine <Character> :: add <Fight_State> ();
template void State_Machine <Character> :: add <Peace_State> ();
