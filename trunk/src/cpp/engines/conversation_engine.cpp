#include <tinyxml.h>

#include "engines/conversation_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "items/characters/player_character.hpp"
#include "messages/conversation_message.hpp"
#include "strategies/actions/conversate.hpp"
#include "strategies/actions/fight.hpp"
#include "strategies/behaviors/ai.hpp"

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
	Object ("conversation engine", true),	//	Here 'true' means 'prevent automatic destruction'.
	behavior
		(new TiXmlDocument (Engines :: Settings :: get () -> radakan_path + "/data/behavior/default_behavior.xml"))
{
	bool check = behavior -> LoadFile ();
	if (behavior -> Error ())
	{
		Engines :: Log :: error (me) << behavior -> ErrorDesc () << endl;
		abort ();
	}
	assert (check);

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
	list_options (Reference <Items :: Characters :: Character> listener)
{
	assert (is_initialized ());

	//	Clear the options from last turn.
	call_observers (Messages :: Message <Items :: Characters :: Character> :: terminate);

	if (listener . points_to_object ())
	{
		TiXmlElement * root = behavior -> RootElement ();
		assert (root != NULL);

		load_options (root, listener);
	}
}

void Conversation_Engine ::
	load_options (const TiXmlElement * element, Reference <Items :: Characters :: Character> listener)
{
	assert (element != NULL);
	
	if (element -> ValueStr () == "option")
	{
		Reference <Messages :: Message <Items :: Characters :: Character> > message
			(new Messages :: Conversation_Message
				(Items :: Characters :: Player_Character :: get (), listener, element));

		call_observers (message);
	}
	
	if ((element -> ValueStr () == "if") && ! evaluate_condition (element, listener))
	{
		return;
	}
	
	for (const TiXmlElement * sub_element = element -> FirstChildElement ();
		sub_element != NULL; sub_element = sub_element -> NextSiblingElement ())
	{
		load_options (sub_element, listener);
	}
}

bool Conversation_Engine ::
	evaluate_condition (const TiXmlElement * element, Reference <Items :: Characters :: Character> subject)
{
	assert (is_initialized ());
	assert (element != NULL);
	
	bool result = true;

	for (const TiXmlAttribute * attribute = element -> FirstAttribute (); attribute != NULL;
		attribute = attribute -> Next ())
	{
		result = result && evaluate_expression (attribute, subject);
	}

	return result;
}

bool Conversation_Engine ::
	evaluate_expression (const TiXmlAttribute * attribute, Reference <Items :: Characters :: Character> subject)
{
	assert (is_initialized ());
	assert (attribute != NULL);
	assert (subject . points_to_object ());
	assert (subject -> is_initialized ());
	assert (! subject -> is_dead ());

	const string & name = attribute -> NameTStr ();
	const string & value = attribute -> ValueStr ();
	Log :: log (me) << "evaluate: " << name  << " ? " << value << endl;

	if (name == "action")
	{
		if (subject -> get_active_state () . is_castable <Strategies :: Behaviors :: AI> ())
		{
			Reference <Strategies :: Behaviors :: AI> ai
				(subject -> get_active_state () . cast <Strategies :: Behaviors :: AI> ());

			Log :: log (me) << "result: " << to_string (ai -> get_current_action_name () == value) << endl;
			return ai -> get_current_action_name () == value;
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

			Log :: log (me) << "result: " << to_string (is_smaller != larger) << endl;
		return (is_smaller != larger);
	}
	
	if ((name == "know") || (name == "have"))
	{
		Log :: log (me) << "result: false (I don't know and don't have.)" << endl;
		return false;
	}
	
	Engines :: Log :: log (me) << "Expression not valid or not implemented." << endl;
	abort ();
	return false;
}
