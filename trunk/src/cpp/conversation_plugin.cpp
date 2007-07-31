#include "chat_state.hpp"
#include "conversation_plugin.hpp"
#include "gui_engine.hpp"
#include "log.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Plugins;

//	static
const string Conversation_Plugin ::
	get_class_name ()
{
	return "Conversation_Plugin";
}

//  constructor
Conversation_Plugin ::
	Conversation_Plugin () :
	Object ("conversation plugin")
{
	//	Do nothing.

	assert (is_initialized ());
}

//  destructor
Conversation_Plugin ::
	~Conversation_Plugin ()
{
	Engines :: Log :: trace (me, Conversation_Plugin :: get_class_name (), "~");
	assert (is_initialized ());
	
	forget_dependencies ();
}

//	virtual
bool Conversation_Plugin ::
	is_initialized ()
	const
{
	assert (Singleton <Conversation_Plugin> :: is_initialized ());
	assert (Observer <Strategies :: Play_State> :: is_initialized ());
	
	return true;
}

//	virtual
void Conversation_Plugin ::
	call (const Object & message)
{
	assert (is_initialized ());
	
/*	if (message == "conversation")
	{
		NPC * npc = dynamic_cast <NPC *> (& message);
		assert (npc != NULL);

		conversations . add (Player_Character :: get (), * npc);
	}

	for (Set <Character> * conversation = conversations . get_child ();
		conversation != NULL; conversation = conversations . get_another_child ())
	{
		Character & one = * conversation -> get_child ();
		Character & two = * conversation -> get_another_child ();
		assert (one . is_initialized ());
		assert (two . is_initialized ());
		assert (one . is_type <Player_Character> ());
		assert (two . is_type <NPC> ());

		if (one . is_dead () || two . is_dead ())
		{
			Log :: show ("You can't talk with the dead.");
		}
		else
		{
			Player_Character :: get () . chat ("hello", two);
		}
	}*/
}
