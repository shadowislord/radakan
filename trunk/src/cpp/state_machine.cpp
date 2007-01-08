#include "state_machine.hpp"

using namespace std;
using namespace tsl;

//  constructor
State_Machine ::
	State_Machine () :
	Object (* this + "'s state machine"),
	State (this -> to_type <Character> ()),
	Set <State> (* this)
{
	trace () << "State_Machine ()" << endl;
	assert (Set <State> :: is_initialized ());

	active_child_state = NULL;
	
	assert (State_Machine :: is_initialized ());
}

//  destructor
State_Machine ::
	~State_Machine ()
{
	assert (State_Machine :: is_initialized ());
}

//	virtual
bool State_Machine ::
	is_initialized ()
	const
{
//	trace () << "State_Machine :: is_initialized () A" << endl;
	assert (warn <State_Machine> (State :: is_initialized ()));
//	trace () << "State_Machine :: is_initialized () B" << endl;
	assert (warn <State_Machine> (Set <State> :: is_initialized ()));
//	trace () << "State_Machine :: is_initialized () C" << endl;

	return (warn <State_Machine> ((active_child_state == NULL) || active_child_state -> is_initialized ()));
}

//	static
string State_Machine ::
	get_type_name ()
{
	return "state machine";
}

//	virtual
string State_Machine ::
	think ()
{
	assert (is_initialized ());
	assert (active_child_state != NULL);
	assert (! owner . is_dead ());
	
	return active_child_state -> think ();
}

//	virtual
bool State_Machine ::
	add (State & state)
{
	assert (is_initialized ());
	assert (state . is_initialized ());

	bool result = Set <State> :: add (state);

	assert (result);

	active_child_state = & state . to_type <State> ();

	return true;
}
