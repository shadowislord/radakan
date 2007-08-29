#include "alive_state.hpp"
#include "chat_state.hpp"
#include "conversation_message.hpp"
#include "fight_state.hpp"
#include "gui.hpp"
#include "log.hpp"
#include "npc.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;

//	static
const unsigned int Chat_State ::
	maximal_timeout = 100;	//	turns, not seconds or something

//	static
const string Chat_State ::
	get_class_name ()
{
	return "Chat_State";
}

//  constructor
Chat_State ::
	Chat_State (Alive_State & new_alive_state) :
	Object ("chat state"),
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
	assert (Strategy :: is_initialized ());
	
	return true;
}

//	virtual
Strategy * Chat_State ::
	transit (const Object & message)
{
	assert (is_initialized ());

	if (message . is_type <Messages :: Conversation_Message> ())
	{
		Messages :: Conversation_Message & conversation_message = message . to_type <Messages :: Conversation_Message> ();
		
		if (conversation_message . to == alive_state . npc)	//	Is (s)he talking to me?
		{
			const TiXmlNode * temp = & conversation_message . option;

			for (int i = 0; temp -> ValueStr () != "dialog"; i ++)
			{
				assert (i < 5);
				
				temp = temp -> Parent ();
				assert (temp != NULL);
			}

			temp = temp -> FirstChild ("reactions");
			assert (temp != NULL);

			for (int i = 0; temp -> ValueStr () != "reaction"; i ++)
			{
				assert (i < 5);
				
				temp = temp -> FirstChild ();
				assert (temp != NULL);
			}

			const TiXmlElement * reaction = temp -> ToElement ();
			assert (reaction != NULL);

			alive_state . npc . chat (* reaction, conversation_message . from);

			timeout = maximal_timeout;
		}
	}

	if (timeout == 0)
	{
		alive_state . npc . think ("I'm not going to wait any longer...");

		alive_state . calm *= 0.9;	//	The NPC gets slightly annoyed.

		return NULL;
	}

	timeout --;
	
	return this;
}

//	static
Strategy & Chat_State ::
	create (Alive_State & new_alive_state)
{
	return static_cast <Strategy &> (* new Chat_State (new_alive_state));
}
