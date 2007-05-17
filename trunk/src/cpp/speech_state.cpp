#include "alive_state.hpp"
#include "chat_state.hpp"
#include "fight_state.hpp"
#include "log.hpp"
#include "npc.hpp"
#include "speech_state.hpp"

using namespace std;
using namespace TSL;

//	static
const string Speech_State ::
	get_class_name ()
{
	return "Speech_State";
}

//  constructor
Speech_State ::
	Speech_State (string new_message) :
	Object (new_message),
	Algorithm <NPC> (Chat_State :: get ())
{
	Log :: trace <Speech_State> (me, "", new_message);
	assert (Algorithm <NPC> :: is_initialized ());

	assert (Speech_State :: is_initialized ());
}

//  destructor
Speech_State ::
	~Speech_State ()
{
	Log :: trace <Speech_State> (me, "~");
	assert (Algorithm <NPC> :: is_initialized ());
}

//	virtual
bool Speech_State ::
	is_initialized ()
	const
{
	Log :: trace <Speech_State> (me, "is_initialized");
	return Algorithm <NPC> :: is_initialized ();
}

//	virtual
Algorithm <NPC> & Speech_State ::
	transit (NPC & owner)
{
	Log :: trace <Speech_State> (me, "transit", owner);
	assert (is_initialized ());

	Log :: show (owner + ": " + * this);

	return Alive_State :: get ();
}

//	static
Algorithm <NPC> & Speech_State ::
	create (string new_message)
{
	Algorithm <NPC> * temp = new Speech_State (new_message);

	return * temp;
}
