#include "chat_state.hpp"
#include "conversation_message.hpp"
#include "fight_state.hpp"
#include "log.hpp"
#include "npc.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Algorithms;

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
	Algorithm <Items :: NPC> (Alive_State :: get ())
{
	//	Do nothing.

	assert (is_initialized ());
}

//  destructor
Chat_State ::
	~Chat_State ()
{
	Engines :: Log :: trace (me, Chat_State :: get_class_name (), "~");
	assert (is_initialized ());

	forget_dependencies ();
}

//	virtual
bool Chat_State ::
	is_initialized ()
	const
{
	assert (Algorithm <Items :: NPC> :: is_initialized ());
	
	return true;
}

//	virtual
Algorithm <Items :: NPC> & Chat_State ::
	transit (Items :: NPC & owner, const Object & message)
{
	assert (is_initialized ());

	if (message . is_type <Messages :: Conversation_Message> ())
	{
		Messages :: Conversation_Message & conversation_message = message . to_type <Messages :: Conversation_Message> ();
		
		if (conversation_message . to == owner)	//	Is (s)he talking to me?
		{
			if (conversation_message == "Hello.")
			{
				owner . chat ("Hello, " + conversation_message . from + "!", conversation_message . from);
				
				owner . conversation_timeout = 30;
			}
			else if (conversation_message == "Duel!")
			{
				return Fight_State :: get ();
			}
		}
	}

	if (owner . conversation_timeout == 0)
	{
		owner . chat ("I'm not going to wait longer...", owner);

		return Alive_State :: get ();
	}

	owner . conversation_timeout --;

	//	Return myself.
	return * this;
}

//	virtual
void Chat_State ::
	enter (Items :: NPC & owner)
{
	owner . conversation_timeout = 30;
}

//	virtual
void Chat_State ::
	exit (Items :: NPC & owner)
{
	owner . conversation_timeout = 0;
}
