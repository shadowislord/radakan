#include <tinyxml.h>

#include "engines/conversation_engine.hpp"
#include "engines/log.hpp"
#include "gui.hpp"
#include "items/character.hpp"
#include "messages/battle_message.hpp"
#include "messages/conversation_message.hpp"
#include "movable_model.hpp"
#include "strategies/actions/conversate.hpp"
#include "strategies/actions/fight.hpp"
#include "strategies/behaviors/ai.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Actions;

//	static
const unsigned int Conversate ::
	maximal_timeout = 100;	//	turns, not seconds or something

//	static
string Conversate ::
	get_class_name ()
{
	return "Conversate";
}

//  constructor
Conversate ::
	Conversate
	(
		Reference <Items :: Character> new_character,
		Reference <Behaviors :: AI> new_ai
	) :
	Object (new_character . get_name () + "'s conversate action"),
	Action (new_character, new_ai),
	timeout (maximal_timeout)
{
	//	Do nothing.

	assert (is_initialized ());
}

//  destructor
Conversate ::
	~Conversate ()
{
	Engines :: Log :: trace (me, Conversate :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Conversate ::
	is_initialized ()
	const
{
	//	'assert' can't handle double templates.
	//	assert (Strategy <Action, Items :: Character> :: is_initialized ());
	
	return true;
}

//	virtual
Reference <Action> Conversate ::
	transit (const Reference <Messages :: Message <Items :: Character> > & message)
{
	assert (is_initialized ());

	if (message -> to == character)	//	Is (s)he talking to me?
	{
		if (message . is_castable <Messages :: Conversation_Message> ())
		{
			const Reference <Messages :: Conversation_Message> conversation_message
				= message . cast_const <Messages :: Conversation_Message> ();
		
			Reference <Messages :: Conversation_Message> reaction
				(conversation_message -> get_reaction ());

			if (reaction -> option -> Attribute ("action") != NULL)
			{
				string new_action_name = reaction -> option -> Attribute ("action");

				if (new_action_name == Fight :: get_class_name ())
				{
					return Reference <Action> (new Fight (character, ai));
				}
				else
				{
					assert (new_action_name == "none");
					
					return Reference <Action> ();
				}
			}
			
			character -> call_observers (reaction);

			timeout = maximal_timeout;
		}
	}

	if (timeout == 0)
	{
		Engines :: Log :: show
			(character . get_name () + ": \"I'm not going to wait any longer...\"");

		ai -> calm *= 0.9;	//	The character gets slightly annoyed.

		return Reference <Action> ();
	}

	timeout --;
	
	return Reference <Action> (this);
}

//	virtual
string Conversate ::
	get_action_name ()
{
	return get_class_name ();
}
