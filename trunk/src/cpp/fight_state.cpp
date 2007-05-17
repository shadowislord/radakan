#include "alive_state.hpp"
#include "fight_state.hpp"
#include "log.hpp"
#include "npc.hpp"

using namespace std;
using namespace TSL;

//	static
const string Fight_State ::
	get_class_name ()
{
	return "Fight_State";
}

//  constructor
Fight_State ::
	Fight_State () :
	Object ("fight state"),
	Algorithm <NPC> (Alive_State :: get ())
{
	assert (Algorithm <NPC> :: is_initialized ());

	assert (Fight_State :: is_initialized ());
}

//  destructor
Fight_State ::
	~Fight_State ()
{
	Log :: trace <Fight_State> (me, "~");
	assert (Algorithm <NPC> :: is_initialized ());
}

//	virtual
bool Fight_State ::
	is_initialized ()
	const
{
	assert (Algorithm <NPC> :: is_initialized ());
	assert (Singleton <Fight_State> :: is_initialized ());
	
	return true;
}

//	virtual
Algorithm <NPC> & Fight_State ::
	transit (NPC & owner)
{
	assert (is_initialized ());

	return owner . get_active_state ();
}
