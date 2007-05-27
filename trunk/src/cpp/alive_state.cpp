#include "alive_state.hpp"
#include "chat_state.hpp"
#include "conversation_message.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "log.hpp"
#include "npc.hpp"
#include "movable_model.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Algorithms;

//	static
const string Alive_State ::
	get_class_name ()
{
	return "Alive_State";
}

//  constructor
Alive_State ::
	Alive_State () :
	Object ("alive state")
{
	//	Do nothing.

	assert (Alive_State :: is_initialized ());
}

//  destructor
Alive_State ::
	~Alive_State ()
{
	Engines :: Log :: trace <Alive_State> (me, "~");
	assert (Alive_State :: is_initialized ());

	// Do nothing.
}

//	virtual
bool Alive_State ::
	is_initialized ()
	const
{
	assert (Algorithm <Items :: NPC> :: is_initialized ());
	
	return true;
}

//	virtual
Algorithm <Items :: NPC> & Alive_State ::
	transit (Items :: NPC & owner, const Object & message)
{
	assert (is_initialized ());
	
	if (message == terminate)
	{
		return Dead_State :: get ();
	}

	owner . get_movable_model () . move (0);
	owner . get_movable_model () . turn (0);

	//	'peace' state:
	if (owner . get_active_state () != Fight_State :: get ())
	{
		if (message . is_type <Messages :: Conversation_Message> ())
		{
			if (message . to_type <Messages :: Conversation_Message> () . to == owner)
			{
				//	The Algorithm_State_machine will re-run, if we're not already chatting.
				return Chat_State :: get ();
			}
		}
	}

	//	Return me or a child state.
	return owner . get_active_state ();
}
