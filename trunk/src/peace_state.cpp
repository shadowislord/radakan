#include "peace_state.hpp"
#include "fight_state.hpp"

using namespace std;

//  constructor
Peace_State ::
	Peace_State (Character * new_owner, State * new_parent_state) :
	Object (* new_owner + "'s peace state"),
	State (new_owner, new_parent_state)
{
	assert (Peace_State :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
Peace_State ::
	~Peace_State ()
{
	assert (Object :: is_initialized (* this + " -> ~Peace_State ()"));
}

//	virtual
bool Peace_State ::
	is_initialized ()
	const
{
	return State :: is_initialized ();
}

//	virtual
string Peace_State ::
	act ()
{
	return "";
}

//	virtual
string Peace_State ::
	think ()
{
	if (owner -> has_weapon ())
	{
		parent_state -> to_type <State_Machine> () -> change_active_state <Fight_State> ();
		return * owner + " gets aggressive!";
	}
	else
	{
		return * owner + " stays calm.";
	}
}
