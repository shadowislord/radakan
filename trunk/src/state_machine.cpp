#include "state_machine.hpp"

using namespace std;

//  constructor
State_Machine ::
	State_Machine (State * new_parent_state) :
	Object (* this + "'s state machine"),
	State (this -> to_type <Character> (), new_parent_state),
	Set <State> (* this)
{
	assert (Set <State> :: is_initialized ());

	active_child_state = NULL;
	
	assert (is_initialized ());
}

//  destructor
State_Machine ::
	~State_Machine ()
{
	assert (is_initialized ());
}

//	virtual
bool State_Machine ::
	is_initialized ()
	const
{
	return Set <State> :: is_initialized () && ((active_child_state == NULL) || active_child_state -> is_initialized ());
}

//	virtual
void State_Machine ::
	act ()
{
	assert (is_initialized ());
	assert (active_child_state != NULL);
	active_child_state -> act ();
}

//	virtual
void State_Machine ::
	think ()
{
	assert (is_initialized ());
	assert (active_child_state != NULL);
	active_child_state -> think ();
}

//	virtual
bool State_Machine ::
	add (State * state)
{
	assert (is_initialized ());
	assert (state != NULL);
	assert (state -> is_initialized ());

	bool result = Set <State> :: add (state);

	assert (result);

	active_child_state = state -> to_type <State> ();

	return true;
}
