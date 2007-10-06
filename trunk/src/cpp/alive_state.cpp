#include "alive_state.hpp"
#include "battle_message.hpp"
#include "chat_state.hpp"
#include "conversation_message.hpp"
#include "fight_state.hpp"
#include "log.hpp"
#include "npc.hpp"
#include "movable_model.hpp"

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
	Alive_State (Reference <Items :: NPC> new_npc) :
	Object ("alive state"),
	npc (new_npc),
	calm (1),
	sensory_buffer (new Set <Object> (new_npc -> name + "'s sensory buffer"))
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

	if (! npc -> is_destructing ())
	{
		npc -> get_movable_model () -> turn (1, npc -> get_model () -> get_side_direction ());
		
		Engines :: Log :: show (npc -> name + " died.");
	}
}

//	virtual
bool Alive_State ::
	is_initialized ()
	const
{
	assert (Strategy :: is_initialized ());
	assert (0 <= calm);
	assert (calm <= 1);

	return true;
}

//	virtual
Reference <Strategy> Alive_State ::
	transit (const Reference <Object> message)
{
	assert (is_initialized ());
	
	if (message == terminate)
	{
		return NULL;
	}

	npc -> get_movable_model () -> move (0);
	npc -> get_movable_model () -> turn (0);

	if (message -> is_class <Messages :: Battle_Message> ())
	{
		if (message -> to_class_const <Messages :: Battle_Message> () -> to == npc)
		{
			if (! (has_active_state () && get_active_state () -> is_class <Fight_State> ()))
			{
				set_active_state (Reference <Strategy> (new Fight_State (Reference <Alive_State> (this))));
			}
		}
		else
		{
			//	Ignore the message.
		}
	}
	else if (message -> is_class <Messages :: Conversation_Message> ())
	{
		if (message -> to_class_const <Messages :: Conversation_Message> () -> to == npc)
		{
			if (0.5 < calm)	//	'listening' state:
			{
				if (! has_active_state ())
				{
					set_active_state (Reference <Strategy> (new Chat_State (Reference <Alive_State> (this))));
				}
			}
			else
			{
				Engines :: Log :: show (npc -> name + " is not listening.");
			}
		}
		else
		{
			//	Ignore the message.
		}
	}

	Strategy_State_Machine :: run (message);

	return Reference <Strategy> (this);
}
