#include "npc.hpp"

using namespace std;
using namespace tsl;

//  constructor
Dead_State ::
	Dead_State (NPC & new_owner) :
	Object (new_owner + "'s dead state"),
	State <NPC> (new_owner)
{
	assert (State <NPC> :: is_initialized ());

	assert (Dead_State :: is_initialized ());
}

//  destructor
Dead_State ::
	~Dead_State ()
{
	trace () << "~Dead_State ()" << endl;
	assert (State <NPC> :: is_initialized ());
}

//	virtual
bool Dead_State ::
	is_initialized ()
	const
{
	return State <NPC> :: is_initialized ();
}

//	static
string Dead_State ::
	get_type_name ()
{
	return "dead state";
}

//	virtual
string Dead_State ::
	run ()
{
	assert (is_initialized ());

	error () << "- The deads can't think." << endl;
	abort ();

	return "The deads can't think.";
}
