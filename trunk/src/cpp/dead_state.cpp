#include "log.hpp"
#include "movable_model.hpp"
#include "npc.hpp"
#include "dead_state.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Algorithms;

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
	//	Do nothing.

	assert (Dead_State :: is_initialized ());
}

//  destructor
Dead_State ::
	~Dead_State ()
{
	Engines :: Log :: trace (me, Dead_State :: get_class_name (), "~");
	assert (Algorithm <Items :: NPC> :: is_initialized ());

	forget_dependencies ();
}

//	virtual
bool Dead_State ::
	is_initialized ()
	const
{
	return Algorithm <Items :: NPC> :: is_initialized ();
}

//	virtual
Algorithm <Items :: NPC> & Dead_State ::
	transit (Items :: NPC & owner, const Object & message)
{
	assert (is_initialized ());

	//	Don't do anything, just stay dead.

	return * this;
}

//	virtual
void Dead_State ::
	enter (Items :: NPC & owner)
{
	Engines :: Log :: trace (me, Dead_State :: get_class_name (), "enter", owner);
	assert (is_initialized ());

	owner . get_movable_model () . turn (1, owner . get_model () . get_side_direction ());

	Engines :: Log :: show (owner + " died.");
}
