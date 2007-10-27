#include <tinyxml.h>

#include <CEGUIExceptions.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxTextItem.h>

#include "engines/conversation_engine.hpp"
#include "engines/gui_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "items/npc.hpp"
#include "messages/conversation_message.hpp"
#include "strategies/alive_state.hpp"
#include "strategies/chat_state.hpp"
#include "strategies/fight_state.hpp"

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

void Conversation_Engine ::
	list_options (Reference <Items :: Character> speaker, Reference <Items :: Character> listener)
{
	assert (is_initialized ());

	//	Clear the options from last turn.
	call_observers (Messages :: Message <Items :: Character> :: terminate);

	TiXmlElement * root = behavior -> RootElement ();
	assert (root != NULL);

	Reference <Messages :: Message <Items :: Character> > message;
	
	for (TiXmlElement * state = root -> FirstChildElement ("if"); state != NULL; state = state -> NextSiblingElement ())
	{
		if (evaluate_condition (state, listener))
		{
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
							continue;
						}

						temp = temp -> FirstChildElement ();
					}

					assert (temp -> ValueStr () == "option");

					message . reset_pointee
						(new Messages :: Conversation_Message (temp, speaker, listener));

					call_observers (message);
				}
			}
		}
	}
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
	Log :: log (me) << "evaluate: " << name  << " ? " << value << endl;

	Reference <Items :: NPC> npc;
	if (subject . is_castable <Items :: NPC> ())
	{
		npc = subject . cast <Items :: NPC> ();
		assert (npc . is_initialized ());
		assert (npc . points_to_object ());
		assert (npc -> is_initialized ());
		
		if (npc -> has_active_state ())
		{
			if (name == "active_state")
			{
				Log :: log (me) << name << " ?= \"" << value << "\" ~ A" << endl;
				return Strategies :: Alive_State :: get_class_name () == value;
			}

			if (name == "active_alive_state")
			{
				assert (npc -> get_active_state () . is_castable <Strategies :: Alive_State> ());
				
				Reference <Strategies :: Alive_State> alive_state (npc -> get_active_state () . cast <Strategies :: Alive_State> ());

				if (alive_state -> has_active_state ())
				{
					if (alive_state -> get_active_state () . is_castable <Strategies :: Chat_State> ())
					{
						Log :: log (me) << name << " ?= \"" << value << "\" ~ B" << endl;
						return Strategies :: Chat_State :: get_class_name () == value;
					}
					else
					{
						assert (alive_state -> get_active_state () . is_castable <Strategies :: Fight_State> ());
						Log :: log (me) << name << " ?= \"" << value << "\" ~ C" << endl;
						return Strategies :: Fight_State :: get_class_name () == value;
					}
				}
				else
				{
					Log :: log (me) << name << " ?= \"" << value << "\" ~ D" << endl;
					return "none" == value;
				}
			}
		}
	}

	if ((value . at (0) == '<') || (value . at (0) == '>'))
	{
		bool larger = (value . at (0) == '>');
		string temp = value;
		temp . erase (temp . begin ());
		float numeric_value = to_float (temp);

		bool is_smaller;
		if (name == "like")
		{
			is_smaller = (numeric_value < 0.5);
		}
		else if (name == "fear")
		{
			is_smaller = (numeric_value < 0.2);
		}
		else
		{
			is_smaller = (numeric_value < subject -> get_skill (name));
		}

		return (is_smaller != larger);
	}
	
	if ((name == "know") || (name == "have"))
	{
		return false;
	}
	
	Engines :: Log :: log (me) << "Expression not valid or not implemented." << endl;
	abort ();
	return false;
}
