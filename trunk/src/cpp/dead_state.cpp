#include "log.hpp"
#include "model.hpp"
#include "npc.hpp"
#include "dead_state.hpp"

using namespace std;
using namespace TSL;

//	static
const string Dead_State ::
	get_class_name ()
{
	return "Dead_State";
}

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
	Log :: trace <Dead_State> (me, "~");
	assert (Algorithm <NPC> :: is_initialized ());

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
	Log :: trace <Dead_State> (me, "enter", owner);
	assert (is_initialized ());

	owner . get_movable_model () . turn (1, 10, owner . get_model () . get_side_direction ());

	Log :: show (owner + " died.");
}
