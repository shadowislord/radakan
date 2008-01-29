#include "engines/log.hpp"
#include "engines/mediator.hpp"
#include "gui.hpp"
#include "items/character.hpp"
#include "messages/communications/converse.hpp"
#include "messages/communications/fight.hpp"
#include "movable_model.hpp"
#include "strategies/actions/conversing.hpp"
#include "strategies/actions/fighting.hpp"
#include "strategies/behaviors/ai.hpp"

#include <tinyxml.h>

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Actions;

//	static
const unsigned int Conversing ::
	maximal_timeout = 100;	//	turns, not seconds or something

//	static
string Conversing ::
	get_class_name ()
{
	return "Strategies :: Actions :: Conversing";
}

//  constructor
Conversing ::
 Conversing
	(
		Reference <Items :: Character> new_character,
		Reference <Behaviors :: AI> new_ai
	) :
	Object (new_character . get_name () + "'s converse action"),
	Action (new_character, new_ai),
	timeout (maximal_timeout)
{
	//	Do nothing.

	assert (is_initialized ());
}

//  destructor
Conversing ::
	~Conversing ()
{
	Engines :: Log :: trace (me, Conversing :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Conversing ::
	is_initialized ()
	const
{
	//	'assert' can't handle double templates.
	//	assert (Strategy <Action, Messages :: Communications :: Communication> :: is_initialized ());
	
	return true;
}

//	virtual
Reference <Action> Conversing ::
	transit (Reference <Messages :: Communications :: Communication> message)
{
	assert (is_initialized ());

	if (message -> to == character)	//	Is (s)he talking to me?
	{
		if (message . is_castable <Messages :: Communications :: Converse> ())
		{
			const Reference <Messages :: Communications :: Converse> conversation_message
				= message . cast_const <Messages :: Communications :: Converse> ();

			Engines :: Mediator :: get()
				-> call_observers <Messages :: Communications :: Communication>
					(conversation_message -> get_reaction ());

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
string Conversing ::
	get_action_name ()
{
	return get_class_name ();
}
