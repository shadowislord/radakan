#include "engines/log.hpp"
#include "items/npc.hpp"
#include "messages/battle_message.hpp"
#include "messages/conversation_message.hpp"
#include "movable_model.hpp"
#include "set.hpp"
#include "strategies/alive_state.hpp"
#include "strategies/chat_state.hpp"
#include "strategies/fight_state.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;

//	static
string Alive_State ::
	get_class_name ()
{
	return "Alive_State";
}

//  constructor
Alive_State ::
	Alive_State (Reference <Items :: Character> new_character) :
	Object ("alive state"),
	character (new_character),
	calm (1),
	sensory_buffer
	(
		new Set <Messages :: Message <Items :: Character> >
			(character . get_name () + "'s sensory buffer")
	),
	opinions (new Set <Opinion> (me . get_name () + "'s opinions"))
{
	//	Do nothing.

	assert (Alive_State :: is_initialized ());
}

//  destructor
Alive_State ::
	~Alive_State ()
{
	Engines :: Log :: trace (me, Alive_State :: get_class_name (), "~");
	assert (Alive_State :: is_initialized ());

	prepare_for_destruction ();

	if (! character -> is_destructing ())
	{
		character -> get_movable_model () -> turn (1, character -> get_model () -> get_side_direction ());
		
		Engines :: Log :: show (character . get_name () + " died.");
	}
}

//	virtual
bool Alive_State ::
	is_initialized ()
	const
{
	assert (Strategy <Items :: Character> :: is_initialized ());
	assert (0 <= calm);
	assert (calm <= 1);

	return true;
}

//	virtual
Reference <Strategy <Items :: Character> > Alive_State ::
	transit (const Reference <Messages :: Message <Items :: Character> > & message)
{
	assert (is_initialized ());
	
	if (message == Messages :: Message <Items :: Character> :: terminate)
	{
		return Reference <Strategy <Items :: Character> > ();
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
			set_active_state_class (Fight_State :: get_class_name ());
		}
		else if (actual_message . is_castable <Messages :: Conversation_Message> ())
		{
			if (0.5 < calm)	//	'listening' state:
			{
				set_active_state_class (Chat_State :: get_class_name ());
			}
			else
			{
				Engines :: Log :: show (character . get_name () + " is not listening.");
			}
		}

		Strategy_State_Machine <Items :: Character> :: run (actual_message);
	}
	else	//	so if 'message != update'
	{
		//	We'll store the message for now, and process it on the next 'update'.
	
		Pointer <Messages :: Message <Items :: Character> > message_copy;

		if (message . is_castable <Messages :: Battle_Message> ())
		{
			const Reference <Messages :: Battle_Message> battle_message (message . cast_const <Messages :: Battle_Message> ());

			message_copy . reset_pointee (new Messages :: Battle_Message (battle_message . get_name (), battle_message -> from, battle_message -> to));
		}
		else if (message . is_castable <Messages :: Conversation_Message> ())
		{
			const Reference <Messages :: Conversation_Message> conversation_message (message . cast_const <Messages :: Conversation_Message> ());

			message_copy . reset_pointee (new Messages :: Conversation_Message (conversation_message -> option, conversation_message -> from, conversation_message -> to));
		}
		else
		{
			Engines :: Log :: error (me) << "Unknown message class." << endl;
			abort ();
		}

		//	Remember the message until next time.
		sensory_buffer -> add (message_copy);
	}

	return Reference <Strategy <Items :: Character> > (this);
}

void Alive_State ::
	set_active_state_class (string class_name)
{
	if (class_name == Chat_State :: get_class_name ())
	{
		if (! (has_active_state () && get_active_state () . is_castable <Chat_State> ()))
		{
			set_active_state (Reference <Strategy <Items :: Character> >
				(new Chat_State (character, Reference <Alive_State> (this))));
		}
	}
	else if (class_name == Fight_State :: get_class_name ())
	{
		if (! (has_active_state () && get_active_state () . is_castable <Fight_State> ()))
		{
			set_active_state (Reference <Strategy <Items :: Character> >
				(new Fight_State (character, Reference <Alive_State> (this))));
		}
	}
	else
	{
		assert (class_name == "none");
		
		if (has_active_state ())
		{
			set_active_state (Reference <Strategy <Items :: Character> > ());
		}
	}
}
