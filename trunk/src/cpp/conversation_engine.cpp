#include <tinyxml.h>

#include <CEGUIExceptions.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxTextItem.h>

#include "alive_state.hpp"
#include "chat_state.hpp"
#include "conversation_engine.hpp"
#include "conversation_message.hpp"
#include "fight_state.hpp"
#include "gui_engine.hpp"
#include "log.hpp"
#include "npc.hpp"
#include "settings.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;

//	static
string Conversation_Engine ::
	get_class_name ()
{
	return "Conversation_Engine";
}

//  constructor
Conversation_Engine ::
	Conversation_Engine () :
	Object ("conversation engine"),
	behavior (new TiXmlDocument (Engines :: Settings :: get () -> radakan_path + "/data/behavior.xml"))
{
	bool check = behavior -> LoadFile ();
	if ((! check) || behavior -> Error ())
	{
		Engines :: Log :: error (me) << behavior -> ErrorDesc () << endl;
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

Reference <Set <Messages :: Conversation_Message> > Conversation_Engine ::
	get_options (Reference <Items :: Character> speaker, Reference <Items :: Character> listener)
{
	assert (is_initialized ());

	TiXmlElement * root = behavior -> RootElement ();
	assert (root != NULL);
	TiXmlElement * state = root -> FirstChildElement ("if");
	assert (state != NULL);
	while (! evaluate_condition (state, listener))
	{
		state = state -> NextSiblingElement ();
	}
	
	Reference <Set <Messages :: Conversation_Message> > result (new Set <Messages :: Conversation_Message> ("conversation options"));

	for (TiXmlElement * dialog = state -> FirstChildElement (); dialog != NULL; dialog = dialog -> NextSiblingElement ())
	{
		TiXmlElement * options = dialog -> FirstChildElement ();
		assert (options != NULL);

		for (TiXmlElement * option = options -> FirstChildElement ();
					option != NULL; option = option -> NextSiblingElement ())
		{
			TiXmlElement * temp = option;
		
			if (temp -> ValueStr () == "if")
			{
				if (! evaluate_condition (temp, speaker))
				{
					break;
				}

				temp = temp -> FirstChildElement ();
			}

			assert (temp -> ValueStr () == "option");

			Reference <Messages :: Conversation_Message> message (new Messages :: Conversation_Message (temp, speaker, listener));

			result -> add (message);
		}
	}

	assert (! result -> is_empty ());

	return result;
}

bool Conversation_Engine ::
	evaluate_condition (const TiXmlElement * element, Reference <Items :: Character> subject)
{
	assert (is_initialized ());
	assert (element != NULL);
	
	bool result = true;

	for (const TiXmlAttribute * attribute = element -> FirstAttribute (); attribute != NULL; attribute = attribute -> Next ())
	{
		result = result && evaluate_expression (attribute, subject);
	}

	return result;
}

bool Conversation_Engine ::
	evaluate_expression (const TiXmlAttribute * attribute, Reference <Items :: Character> subject)
{
	assert (is_initialized ());
	assert (attribute != NULL);

	const string & name = attribute -> NameTStr ();
	const string & value = attribute -> ValueStr ();

	Reference <Items :: NPC> npc;
	if (subject . is_castable <Items :: NPC> ())
	{
		npc = subject . cast <Items :: NPC> ();
	}

	if (name == "active_state")
	{
		if (npc . points_to_object ())
		{
			if (npc -> has_active_state ())
			{
				assert (npc -> get_active_state () . is_castable <Strategies :: Alive_State> ());
				//	Log :: log (me) << name << " ?= \"" << value << "\" ~ A" << endl;
				return Strategies :: Alive_State :: get_class_name () == value;
			}
			else
			{
				return "none" == value;
			}
		}

		//	Log :: log (me) << name << " != \"" << value << "\" ~ C" << endl;
		return false;
	}

	if (name == "active_alive_state")
	{
		if (npc . points_to_object ())
		{
			if (! npc -> has_active_state ())
			{
				//	Log :: log (me) << name << " != \"" << value << "\" ~ D" << endl;
				return false;
			}
			else if (npc -> get_active_state () . points_to_object ())
			{
				assert (npc -> get_active_state () . is_castable <Strategies :: Alive_State> ());
				
				Reference <Strategies :: Alive_State> alive_state (npc -> get_active_state () . cast <Strategies :: Alive_State> ());
			
				if (! alive_state -> has_active_state ())
				{
					//	Log :: log (me) << name << " ?= \"" << value << "\" ~ E" << endl;
					return "none" == value;
				}
				else if (alive_state -> get_active_state () . is_castable <Strategies :: Chat_State> ())
				{
					//	Log :: log (me) << name << " ?= \"" << value << "\" ~ F" << endl;
					return Strategies :: Chat_State :: get_class_name () == value;
				}
				else if (alive_state -> get_active_state () . is_castable <Strategies :: Fight_State> ())
				{
					//	Log :: log (me) << name << " ?= \"" << value << "\" ~ G" << endl;
					return Strategies :: Fight_State :: get_class_name () == value;
				}
			}
		}

		//	Log :: log (me) << name << " != \"" << value << "\" ~ H" << endl;
		return false;
	}

	if (value . at (0) == '<')
	{
		string temp = value;
		temp . erase (temp . begin ());
		
		if (name == "like")
		{
			
		}

		return subject -> get_skill (name) < to_float (temp);
	}
	
	if (value . at (0) == '>')
	{
		string temp = value;
		temp . erase (temp . begin ());

		return subject -> get_skill (name) > to_float (temp);
	}
	
	Engines :: Log :: log (me) << "Expression not valid or not implemented." << endl;
	abort ();
	return false;
}
