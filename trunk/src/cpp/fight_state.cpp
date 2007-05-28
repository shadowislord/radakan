#include "alive_state.hpp"
#include "fight_state.hpp"
#include "log.hpp"
#include "npc.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Algorithms;

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
	Algorithm <Items :: NPC> (Alive_State :: get ())
{
	//	Do nothing.
	
	assert (Fight_State :: is_initialized ());
}

//  destructor
Fight_State ::
	~Fight_State ()
{
	Engines :: Log :: trace (me, Fight_State :: get_class_name (), "~");
	assert (Fight_State :: is_initialized ());

	forget_dependencies ();
}

//	virtual
bool Fight_State ::
	is_initialized ()
	const
{
	assert (Algorithm <Items :: NPC> :: is_initialized ());
	assert (Singleton <Fight_State> :: is_initialized ());
	
	return true;
}

//	virtual
Algorithm <Items :: NPC> & Fight_State ::
	transit (Items :: NPC & owner, const Object & message)
{
	assert (is_initialized ());

	//	Return me or a child state.
	return owner . get_active_state ();
}
