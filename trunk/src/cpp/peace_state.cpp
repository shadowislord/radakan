#include "npc.hpp"

using namespace std;
using namespace tsl;

//  constructor
Peace_State ::
	Peace_State (NPC & new_owner) :
	Object (new_owner + "'s peace state"),
	State <NPC> (new_owner)
{
	assert (State <NPC> :: is_initialized ());

	assert (Peace_State :: is_initialized ());
}

//  destructor
Peace_State ::
	~Peace_State ()
{
	trace () << "~Peace_State ()" << endl;
	
	assert (State <NPC> :: is_initialized ());
}

//	virtual
bool Peace_State ::
	is_initialized ()
	const
{
	return State <NPC> :: is_initialized () && (! owner . is_dead ());
}

//	static
string Peace_State ::
	get_type_name ()
{
	return "peace state";
}

//	virtual
string Peace_State ::
	run ()
{
	assert (is_initialized ());
	
	if (owner . has_weapon ())
	{
		owner . to_type <State_Machine <NPC> > () . change_active_state <Fight_State> ();
		return owner + " gets aggressive!";
	}
	else
	{
		return State <NPC> :: nothing;
	}
}
