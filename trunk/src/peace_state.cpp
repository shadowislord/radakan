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
void Peace_State ::
	act ()
{
}

//	virtual
void Peace_State ::
	think ()
{
	if (owner -> has_weapon ())
	{
		debug () << * owner << " gets aggressive!" << endl;

		parent_state -> to_type <State_Machine> () -> change_active_state <Fight_State> ();
	}
	else
	{
		debug () << * owner << " stays calm." << endl;
	}
}
