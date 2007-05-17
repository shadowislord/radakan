#include "chat_state.hpp"
#include "fight_state.hpp"
#include "log.hpp"
#include "npc.hpp"

using namespace std;
using namespace TSL;

//	static
const string Chat_State ::
	get_class_name ()
{
	return "Chat_State";
}

//  constructor
Chat_State ::
	Chat_State () :
	Object ("chat state"),
	Algorithm <NPC> (Alive_State :: get ())
{
	assert (Algorithm <NPC> :: is_initialized ());

	assert (Chat_State :: is_initialized ());
}

//  destructor
Chat_State ::
	~Chat_State ()
{
	Log :: trace <Chat_State> (me, "~");
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
