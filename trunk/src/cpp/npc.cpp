#include "alive_state.hpp"
#include "battle_message.hpp"
#include "conversation_message.hpp"
#include "log.hpp"
#include "npc.hpp"
#include "thought.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Items;

//	static
string NPC ::
	get_class_name ()
{
	return "NPC";
}

//  constructor
NPC ::
	NPC
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass
	) :
	Object (new_name),
	Character
	(
		new_mesh_name,
		new_size,
		new_mass
	),
	thoughts (name + "'s thoughts")
{
	Engines :: Log :: trace (me, NPC :: get_class_name (), "", new_name, new_mesh_name, to_string (new_size), to_string (new_mass));

	set_active_state (Reference <Strategies :: Strategy> (new Strategies :: Alive_State (Reference <NPC> (this))));

	assert (is_initialized ());
	Engines :: Log :: log (me) << "I'm fully constructed (as NPC)." << endl;
}

//  destructor
NPC ::
	~NPC ()
{
	Engines :: Log :: trace (me, NPC :: get_class_name (), "~");
	assert (NPC :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool NPC ::
	is_initialized ()
	const
{
	assert (Character :: is_initialized ());
	
	return true;
}

//	virtual
void NPC ::
	call (Reference <const Object> message)
{
	assert (is_initialized ());

	if (! is_dead ())
	{
		Reference <Set <const Object> > sensory_buffer (get_active_state () -> to_type <Strategies :: Alive_State> () -> sensory_buffer);
	
		if (message == update)
		{
			if (sensory_buffer -> is_empty ())
			{
				Strategies :: Strategy_State_Machine :: run (update);
			}
			else
			{
				Reference <const Object> buffered_message (sensory_buffer -> get_child ());

				Strategies :: Strategy_State_Machine :: run (buffered_message);
			}
		}
		else if (message -> is_type <Messages :: Battle_Message> ())
		{
			Reference <const Messages :: Battle_Message> battle_message (message -> to_type <const Messages :: Battle_Message> ());
			
			Reference <Messages :: Battle_Message> message_copy (new Messages :: Battle_Message (battle_message -> name, battle_message -> from, battle_message -> to));
		
			//	Remember the message until next time.
			sensory_buffer -> add (message_copy);
		}
		else if (message -> is_type <Messages :: Conversation_Message> ())
		{
			Reference <const Messages :: Conversation_Message> conversation_message (message -> to_type <const Messages :: Conversation_Message> ());
			
			Reference <Messages :: Conversation_Message> message_copy (new Messages :: Conversation_Message (conversation_message -> option, conversation_message -> from, conversation_message -> to));
		
			//	Remember the message until next time.
			sensory_buffer -> add (message_copy);
		}
		else
		{
			Engines :: Log :: error (me) << "Unknown message." << endl;
			abort ();
		}
	}
}

//	virtual
bool NPC ::
	is_dead () const
{
	return ! has_active_state ();
}

//	virtual
void NPC ::
	die ()
{
	assert (NPC :: is_initialized ());
	assert (has_model ());

	call (terminate);

	assert (is_dead ());
}

//	virtual
void NPC ::
	drop (Reference <Strategies :: Strategy> dropped)
{
	Engines :: Log :: trace (this -> me, NPC :: get_class_name (), "drop", dropped -> name);
	assert (NPC :: is_initialized ());

	Strategies :: Strategy_State_Machine :: drop (dropped);
}

//	virtual
void NPC ::
	drop (Reference <Item> dropped)
{
	Engines :: Log :: trace (this -> me, NPC :: get_class_name (), "drop", dropped -> name);
	assert (NPC :: is_initialized ());

	Container :: drop (dropped);
}

void NPC ::
	think (string thought)
	const
{
	Engines :: Log :: show (name + " thinks \"" + thought + "\"");
}
