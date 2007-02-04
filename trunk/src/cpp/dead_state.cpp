#include "npc.hpp"
#include "dead_state.hpp"

using namespace std;
using namespace tsl;

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
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (Algorithm <NPC> :: is_initialized ());
}

//	virtual
bool Dead_State ::
	is_initialized ()
	const
{
	return Algorithm <NPC> :: is_initialized ();
}

//	static
string Dead_State ::
	get_class_name ()
{
	return "Dead_State";
}

//	virtual
Algorithm <NPC> & Dead_State ::
	transit (NPC & owner)
{
	assert (is_initialized ());

	owner . get_representation () . set_orientation (make_quaternion (- Ogre :: Math :: HALF_PI, owner . get_representation () . get_side_direction ()));

	return * this;
}
