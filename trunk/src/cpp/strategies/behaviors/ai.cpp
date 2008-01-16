#include "engines/log.hpp"
#include "items/character.hpp"
#include "messages/battle_message.hpp"
#include "messages/conversation_message.hpp"
#include "movable_model.hpp"
#include "set.hpp"
#include "strategies/actions/conversate.hpp"
#include "strategies/actions/fight.hpp"
#include "strategies/behaviors/ai.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Behaviors;

//	static
string AI ::
	get_class_name ()
{
	return "AI";
}

//  constructor
AI ::
	AI (Reference <Items :: Character> new_character) :
	Object (new_character . get_name () + "'s AI"),
	Behavior (new_character),
	calm (1, 0, 1),
	sensory_buffer
	(
		new Set <Messages :: Message <Items :: Character> >
			(me . get_name () + "'s sensory buffer")
	),
	opinions (new Set <Opinion> (me . get_name () + "'s opinions"))
{
	//	Do nothing.

	assert (AI :: is_initialized ());
}

//  destructor
AI ::
	~AI ()
{
	Engines :: Log :: trace (me, AI :: get_class_name (), "~");
	assert (AI :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool AI ::
	is_initialized ()
	const
{
	//	'assert' can't handle double templates.
	//	assert (Strategy <Behavior, Items :: Character> :: is_initialized ());

	return true;
}

//	virtual
Reference <Behavior> AI ::
	transit (const Reference <Messages :: Message <Items :: Character> > & message)
{
	assert (is_initialized ());
	
	if (message == Messages :: Message <Items :: Character> :: terminate)
	{
		return Reference <Behavior> ();
	}
	else if (message == Messages :: Message <Items :: Character> :: update)
	{
		Pointer <Messages :: Message <Items :: Character> > actual_message (message);
		
		if (! sensory_buffer -> is_empty ())
		{
			actual_message = sensory_buffer -> get_child ();

			sensory_buffer -> drop (actual_message);
		}

		character -> get_movable_model () -> move (0);
		character -> get_movable_model () -> turn (0);

		if (actual_message . is_castable <Messages :: Battle_Message> ())
		{
			set_action (Actions :: Fight :: get_class_name ());
		}
		else if (actual_message . is_castable <Messages :: Conversation_Message> ())
		{
			if (calm > 0.5)	//	'listening' state:
			{
				set_action (Actions :: Conversate :: get_class_name ());
			}
			else
			{
				Engines :: Log :: show (character . get_name (true) + " is not listening.");
			}
		}

		Strategy_State_Machine <Actions :: Action, Items :: Character> :: run
			(actual_message);
	}
	else
	{
		//	We'll store a copy of the message for now, and process it on the next 'update'.
		//	We can't store the message itself, because that's const.
		sensory_buffer -> add (message -> copy ());
	}

	return Reference <Behavior> (this);
}

string AI ::
	get_current_action_name ()
	const
{
	if (has_active_state ())
	{
		return get_active_state () -> get_action_name ();
	}
	else
	{
		return "none";
	}
}

void AI ::
	set_action (string action_name)
{
	if (action_name != get_current_action_name ())
	{
		if (action_name == Actions :: Conversate :: get_class_name ())
		{
			set_active_state (Reference <Actions :: Action>
				(new Actions :: Conversate (character, Reference <AI> (this))));
		}
		else if (action_name == Actions :: Fight :: get_class_name ())
		{
			set_active_state (Reference <Actions :: Action>
				(new Actions :: Fight (character, Reference <AI> (this))));
		}
		else
		{
			assert (action_name == "none");
			
			set_active_state (Reference <Actions :: Action> ());
		}
	}
}
