#include "fight_state.hpp"
#include "peace_state.hpp"

using namespace std;

//  constructor
Fight_State ::
	Fight_State (Character * new_owner, State * new_parent_state) :
	Object (* new_owner + "'s fight state"),
	State (new_owner, new_parent_state)
{
	assert (State :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
Fight_State ::
	~Fight_State ()
{
	assert (Object :: is_initialized (* this + " -> ~Fight_State ()"));
}

//	virtual
bool Fight_State ::
	is_initialized ()
	const
{
	return State :: is_initialized () && (parent_state != NULL);
}

//	virtual
void Fight_State ::
	act ()
{
	debug () << "Attack!" << endl;
}

//	virtual
void Fight_State ::
	think ()
{
	if (! owner -> has_weapon ())
	{
		debug () << "There's no point in being aggressive without a weapon..." << endl;

		parent_state -> to_type <State_Machine> () -> change_active_state <Peace_State> ();
	}
	else
	{
		debug () << "Let's attack..." << endl;
	}
}
