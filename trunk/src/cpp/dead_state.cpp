#include "body.hpp"
#include "npc.hpp"
#include "dead_state.hpp"

using namespace std;
using namespace TSL;

//	static
const string Dead_State ::
	class_name ("Dead_State");

//  constructor
Dead_State ::
	Dead_State () :
	Object ("dead state")
{
	assert (Algorithm <NPC> :: is_initialized ());

	assert (Dead_State :: is_initialized ());
}

//  destructor
Dead_State ::
	~Dead_State ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
	assert (Algorithm <NPC> :: is_initialized ());
}

//	virtual
bool Dead_State ::
	is_initialized ()
	const
{
	return Algorithm <NPC> :: is_initialized ();
}

//	virtual
void Dead_State ::
	enter (NPC & owner)
{
	assert (is_initialized ());

	owner . get_movable_body () . turn (10000, 1, owner . get_body () . get_side_direction ());

	show (owner + " died.");
}
