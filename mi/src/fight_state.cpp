#include "fight_state.hpp"
#include "peace_state.hpp"

using namespace std;

//  constructor
Fight_State ::
	Fight_State (NPC * new_owner):
	Object (* new_owner + "'s fight state"),
	State (new_owner)
{
	assert (State :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
Fight_State ::
	~Fight_State ()
{
	assert (Object :: is_initialized (* this + "->~Fight_State ()"));
}

//	virtual
bool Fight_State ::
	is_initialized ()
	const
{
	return State :: is_initialized ();
}

//	virtual
void Fight_State ::
	act ()
{
	debug () << "Attack!" << endl;
}

//	virtual
void Fight_State ::
	think (State * my_parent)
{
	assert (this->is_in (my_parent));

	if (! owner->has_weapon ())
	{
		debug () << "There's no point in being aggressive without a weapon..." << endl;

		owner->to_type <NPC> ()->change_active_state (my_parent->to_type <State_Machine> ()->get_child <Peace_State> ());
	}
	else
	{
		debug () << "Let's attack..." << endl;
	}
}
