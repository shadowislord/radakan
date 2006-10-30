#include "state.hpp"

using namespace std;
using namespace sl;

//  constructor
State ::
	State (Character * new_owner, State * new_parent_state) :
	Object (* new_owner + "'s generic state")
{
	assert (Object :: is_initialized ());
	assert (new_owner != NULL);
	assert (new_owner -> is_initialized ());
	assert ((new_parent_state == NULL) || new_parent_state -> is_initialized ());
	
	if (new_parent_state == NULL)
	{
		debug () << * this << " -> State (" << * new_owner << ", " << "NULL" << ")"<< endl;
	}
	else
	{
		debug () << * this << " -> State (" << * new_owner << ", " << * new_parent_state << ")"<< endl;
	}
	
	parent_state = new_parent_state;
	owner = new_owner;

	assert (is_initialized ());
}

//  destructor
State ::
	~State ()
{
	assert (Object :: is_initialized (* this + " -> ~State ()"));
}

//	virtual
bool State ::
	is_initialized ()
	const
{
	bool result = Object :: is_initialized ();
	return result && (owner != NULL);
}

//	virtual
string State ::
	act ()
{
	assert (is_initialized ());

	abort ();
	return "";
}

//	virtual
string State ::
	think ()
{
	assert (is_initialized ());
	
	abort ();
	return "";
}
