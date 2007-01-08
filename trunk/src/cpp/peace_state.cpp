#include "dead_state.hpp"
#include "fight_state.hpp"
#include "peace_state.hpp"

using namespace std;
using namespace tsl;

//  constructor
Peace_State ::
	Peace_State (Character & new_owner) :
	Object (new_owner + "'s peace state"),
	State (new_owner)
{
	assert (Peace_State :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
Peace_State ::
	~Peace_State ()
{
	trace () << "~Peace_State ()" << endl;
	assert (Object :: is_initialized ());
}

//	virtual
bool Peace_State ::
	is_initialized ()
	const
{
	return State :: is_initialized () && (! owner . is_dead ());
}

//	static
string Peace_State ::
	get_type_name ()
{
	return "peace state";
}

//	virtual
string Peace_State ::
	think ()
{
	assert (is_initialized ());
	
	if (owner . has_weapon ())
	{
		owner . to_type <State_Machine> () . change_active_state <Fight_State> ();
		return owner + " gets aggressive!";
	}
	else
	{
		return owner + " stays calm.";
	}
}
