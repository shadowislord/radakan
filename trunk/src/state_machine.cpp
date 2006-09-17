#include "state_machine.hpp"

using namespace std;

//  constructor
State_Machine ::
	State_Machine (Character * new_owner, State * new_parent_state) :
	Object (* new_owner + "'s state machine"),
	State (new_owner, new_parent_state),
	Tree (* this)
{
	assert (State :: is_initialized ());
	assert (Tree :: is_initialized ());

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
	return State :: is_initialized () && Tree :: is_initialized () && ((active_child_state == NULL) || active_child_state -> is_initialized ());
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

//	returns subtree, iff succes
//	virtual
bool State_Machine ::
	add (Object * sub_tree)
{
	assert (is_initialized ());
	assert (sub_tree != NULL);
	assert (sub_tree -> is_initialized ());
	assert (sub_tree -> is_type <State> ());

	bool result = Tree :: add (sub_tree);

	assert (result);

	if (active_child_state == NULL)
	{
		active_child_state = sub_tree -> to_type <State> ();
	}
	
	return true;
}
