#include "npc.hpp"

using namespace std;
using namespace tsl;

//  constructor
Fight_State ::
	Fight_State (NPC & new_owner) :
	Object (new_owner + "'s fight state"),
	State <NPC> (new_owner)
{
	assert (State <NPC> :: is_initialized ());

	assert (Fight_State :: is_initialized ());
}

//  destructor
Fight_State ::
	~Fight_State ()
{
	trace () << "~Fight_State ()" << endl;
	
	assert (State <NPC> :: is_initialized ());
}

//	virtual
bool Fight_State ::
	is_initialized ()
	const
{
	return State <NPC> :: is_initialized () && (! owner . is_dead ());
}

//	static
string Fight_State ::
	get_type_name ()
{
	return "fight state";
}

//	virtual
string Fight_State ::
	run ()
{
	assert (is_initialized ());

	if (! owner . has_weapon ())
	{
		owner . to_type <State_Machine <NPC> > () . change_active_state <Peace_State> ();
		return owner + " says 'There's no point in being aggressive without a weapon.'";
	}
	else
	{
		return State <NPC> :: nothing;
	}
}
