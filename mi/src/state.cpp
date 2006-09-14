#include "state.hpp"

using namespace std;

//  constructor
State ::
	State (Character * new_owner):
	Object (* new_owner + "'s generic state")
{
	assert (Object :: is_initialized ());

	owner = new_owner;

	assert (is_initialized ());
}

//  destructor
State ::
	~State ()
{
	assert (Object :: is_initialized (* this + "->~State ()"));
}

//	virtual
bool State ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	virtual
void State ::
	act ()
{
	debug () << "default AI state acting..." << endl;
}

//	virtual
void State ::
	think (State * my_parent)
{
	debug () << "default AI state thinking..." << endl;
}

//virtual
void State ::
	change_active_state (State * new_state)
{
	abort ();
}

