#include "alive_state.hpp"
#include "chat_state.hpp"
#include "conversation_engine.hpp"
#include "fight_state.hpp"
#include "gui_engine.hpp"
#include "log.hpp"
#include "npc.hpp"
#include "settings.hpp"

#include <CEGUIExceptions.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxTextItem.h>

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;

//	static
const string Conversation_Engine ::
	get_class_name ()
{
	return "Conversation_Engine";
}

//  constructor
Conversation_Engine ::
	Conversation_Engine () :
	Object ("conversation engine"),
	behavior (Engines :: Settings :: get () . radakan_path + "/data/behavior.xml")
{
	bool check = behavior . LoadFile ();
	if ((! check) || behavior . Error ())
	{
		Engines :: Log :: error (me) << behavior . ErrorDesc () << endl;
		abort ();
	}

	assert (is_initialized ());
}

//  destructor
Conversation_Engine ::
	~Conversation_Engine ()
{
	Engines :: Log :: trace (me, Conversation_Engine :: get_class_name (), "~");
	assert (is_initialized ());
	
	prepare_for_destruction ();
}

//	virtual
bool Conversation_Engine ::
	is_initialized ()
	const
{
	assert (Singleton <Conversation_Engine> :: is_initialized ());

	return true;
}

Set <Messages :: Conversation_Message> & Conversation_Engine ::
	get_options (Items :: Character & speaker, Items :: Character & listener)
{
	assert (is_initialized ());

	TiXmlElement * root = behavior . RootElement ();
	assert (root != NULL);
	TiXmlElement * state = root -> FirstChildElement ("if");
	assert (state != NULL);
	while (! evaluate_condition (state, listener))
	{
		state = state -> NextSiblingElement ();
		assert (state != NULL);
	}
	
	Set <Messages :: Conversation_Message> * result = new Set <Messages :: Conversation_Message> ("conversation options");

	for (TiXmlElement * dialog = state -> FirstChildElement (); dialog != NULL; dialog = dialog -> NextSiblingElement ())
	{
		TiXmlElement * options = dialog -> FirstChildElement ();
		assert (options != NULL);

		for (TiXmlElement * option = options -> FirstChildElement ("option");
					option != NULL; option = option -> NextSiblingElement ("option"))
		{
			Messages :: Conversation_Message * message = new Messages :: Conversation_Message (* option, speaker, listener);

			result -> add (* message);
		}
	}

	assert (! result -> is_empty ());

	return * result;
}

bool Conversation_Engine ::
	evaluate_condition (const TiXmlElement * element, Items :: Character & subject)
{
	assert (element != NULL);
	
	bool result = true;

	for (const TiXmlAttribute * attribute = element -> FirstAttribute (); attribute != NULL; attribute = attribute -> Next ())
	{
		bool temp = evaluate_expression (attribute, subject);
		if (temp)
		{
			Log :: log (me) << "true" << endl;
		}
		else
		{
			Log :: log (me) << "false" << endl;
		}
		result = result && temp;
	}

	return result;
}

bool Conversation_Engine ::
	evaluate_expression (const TiXmlAttribute * attribute, Items :: Character & subject)
{
	assert (attribute != NULL);

	const string & name = attribute -> NameTStr ();
	const string & value = attribute -> ValueStr ();

	if (name == "active_state")
	{
		if (subject . is_type <Items :: NPC> ())
		{
			Strategies :: Strategy_State_Machine & npc = subject . to_type <Strategies :: Strategy_State_Machine> ();
			
			if (! npc . has_active_state ())
			{
				Log :: log (me) << name << " ?= \"" << value << "\" ~ A" << endl;
				return "none" == value;
			}
			else if (npc . get_active_state () . is_type <Strategies :: Alive_State> ())
			{
				Log :: log (me) << name << " ?= \"" << value << "\" ~ B" << endl;
				return Strategies :: Alive_State :: get_class_name () == value;
			}
		}

		Log :: log (me) << name << " != \"" << value << "\" ~ C" << endl;
		return false;
	}

	if (name == "active_alive_state")
	{
		if (subject . is_type <Items :: NPC> ())
		{
			Strategies :: Strategy_State_Machine & npc = subject . to_type <Strategies :: Strategy_State_Machine> ();
			
			if (! npc . has_active_state ())
			{
				Log :: log (me) << name << " != \"" << value << "\" ~ D" << endl;
				return false;
			}
			else if (npc . get_active_state () . is_type <Strategies :: Alive_State> ())
			{
				Strategies :: Strategy_State_Machine & alive_state = npc . get_active_state () . to_type <Strategies :: Strategy_State_Machine> ();
			
				if (! alive_state . has_active_state ())
				{
					Log :: log (me) << name << " ?= \"" << value << "\" ~ E" << endl;
					return "none" == value;
				}
				else if (alive_state . get_active_state () . is_type <Strategies :: Chat_State> ())
				{
					Log :: log (me) << name << " ?= \"" << value << "\" ~ F" << endl;
					return Strategies :: Chat_State :: get_class_name () == value;
				}
				else if (alive_state . get_active_state () . is_type <Strategies :: Fight_State> ())
				{
					Log :: log (me) << name << " ?= \"" << value << "\" ~ G" << endl;
					return Strategies :: Fight_State :: get_class_name () == value;
				}
			}
		}

		Log :: log (me) << name << " != \"" << value << "\" ~ H" << endl;
		return false;
	}

	
	Engines :: Log :: log (me) << "Expression not valid or not implemented." << endl;
	abort ();
	return false;
}
