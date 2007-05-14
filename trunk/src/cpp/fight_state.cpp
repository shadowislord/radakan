#include "npc.hpp"
#include "fight_state.hpp"
#include "alive_state.hpp"

using namespace std;
using namespace TSL;

//	static
const string Fight_State ::
	class_name ("Fight_State");

//  constructor
Fight_State ::
	Fight_State () :
	Object ("fight state"),
	Algorithm <NPC> (& Alive_State :: get ())
{
	assert (Algorithm <NPC> :: is_initialized ());

	assert (Fight_State :: is_initialized ());
}

//  destructor
Fight_State ::
	~Fight_State ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
	assert (Algorithm <NPC> :: is_initialized ());
}

//	virtual
bool Fight_State ::
	is_initialized ()
	const
{
	return Algorithm <NPC> :: is_initialized ();
}

//	virtual
Algorithm <NPC> & Fight_State ::
	transit (NPC & owner)
{
	assert (is_initialized ());

	return owner . get_active_state ();
}
