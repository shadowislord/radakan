#include "state_machine.hpp"

using namespace std;

//  constructor
State_Machine ::
	State_Machine (NPC * new_owner):
	Object (* new_owner + "'s state machine"),
	State (new_owner),
	Tree (* this)
{
	assert (State :: is_initialized ());
	assert (Tree :: is_initialized ());

	active_state = NULL;
	
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
	return State :: is_initialized () && Tree :: is_initialized () && ((active_state == NULL) || active_state->is_initialized ());
}

//	virtual
void State_Machine ::
	act ()
{
	assert (is_initialized ());
	assert (active_state != NULL);
	active_state->act ();
}

//	virtual
void State_Machine ::
	think (State * my_parent)
{
	assert (is_initialized ());
	assert (active_state != NULL);
	active_state->think (this);
}

//virtual
void State_Machine ::
	change_active_state (State * new_state)
{
	assert (is_initialized ());
	assert (contains (new_state, false));

	active_state = new_state;
}

//	returns subtree, iff succes
//	virtual
bool State_Machine ::
	add (Object * sub_tree)
{
	assert (is_initialized ());
	assert (sub_tree != NULL);
	assert (sub_tree->is_initialized ());
	assert (sub_tree -> is_type <State> ());

	bool result = Tree :: add (sub_tree);

	assert (result);

	if (active_state == NULL)
	{
		active_state = sub_tree->to_type <State> ();
	}
	
	return true;
}
