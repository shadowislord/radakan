#include "npc.hpp"
#include "chat_state.hpp"
#include "fight_state.hpp"
#include "alive_state.hpp"

using namespace std;
using namespace TSL;

//	static
const string Chat_State ::
	class_name ("Chat_State");

//  constructor
Chat_State ::
	Chat_State () :
	Object ("chat state"),
	Algorithm <NPC> (& Alive_State :: get ())
{
	assert (Algorithm <NPC> :: is_initialized ());

	assert (Chat_State :: is_initialized ());
}

//  destructor
Chat_State ::
	~Chat_State ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
	assert (Algorithm <NPC> :: is_initialized ());
}

//	virtual
bool Chat_State ::
	is_initialized ()
	const
{
	return Algorithm <NPC> :: is_initialized ();
}

//	virtual
Algorithm <NPC> & Chat_State ::
	transit (NPC & owner)
{
	assert (is_initialized ());

	return owner . get_active_state ();
}
