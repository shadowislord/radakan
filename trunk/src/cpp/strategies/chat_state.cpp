#include <tinyxml.h>

#include "engines/conversation_engine.hpp"
#include "engines/log.hpp"
#include "gui.hpp"
#include "items/npc.hpp"
#include "messages/battle_message.hpp"
#include "messages/conversation_message.hpp"
#include "movable_model.hpp"
#include "strategies/alive_state.hpp"
#include "strategies/chat_state.hpp"
#include "strategies/fight_state.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;

//	static
const unsigned int Chat_State ::
	maximal_timeout = 100;	//	turns, not seconds or something

//	static
string Chat_State ::
	get_class_name ()
{
	return "Chat_State";
}

//  constructor
Chat_State ::
	Chat_State (Reference <Items :: Character> new_character, Reference <Alive_State> new_alive_state) :
	Object ("chat state"),
	character (new_character),
	alive_state (new_alive_state),
	timeout (maximal_timeout)
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

	prepare_for_destruction ();
}

//	virtual
bool Chat_State ::
	is_initialized ()
	const
{
	assert (Strategy <Items :: Character> :: is_initialized ());
	
	return true;
}

//	virtual
Reference <Strategy <Items :: Character> > Chat_State ::
	transit (const Reference <Messages :: Message <Items :: Character> > & message)
{
	assert (is_initialized ());

	if (message . is_castable <Messages :: Conversation_Message> ())
	{
		const Reference <Messages :: Conversation_Message> conversation_message = message . cast_const <Messages :: Conversation_Message> ();
		
		if (conversation_message -> to == character)	//	Is (s)he talking to me?
		{
			Reference <Messages :: Conversation_Message> reaction
				(conversation_message -> get_reaction ());

			if (reaction -> option -> Attribute ("active_alive_state") != NULL)
			{
				string new_state_class_name = reaction -> option -> Attribute ("active_alive_state");

				if (new_state_class_name == Fight_State :: get_class_name ())
				{
					return Reference <Strategy <Items :: Character> > (new Fight_State (character, alive_state));
				}
				else
				{
					assert (new_state_class_name == "none");
					
					return Reference <Strategy <Items :: Character> > ();
				}
			}
			
			character -> call_observers (reaction);

			timeout = maximal_timeout;
		}
	}

	if (timeout == 0)
	{
		Engines :: Log :: log (me) << character << ": I'm not going to wait any longer..." << endl;

		alive_state -> calm *= 0.9;	//	The NPC gets slightly annoyed.

		return Reference <Strategy <Items :: Character> > ();
	}

	timeout --;
	
	return Reference <Strategy <Items :: Character> > (this);
}
