#include <tinyxml.h>

#include "engines/conversation_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "items/character.hpp"
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
	list_player_options ()
{
	assert (is_initialized ());

	//	Clear the options from last turn.
	call_observers (Messages :: Message <Items :: Character> :: terminate);

	Reference <Items :: Character> npc
		= Items :: Character :: get_player_character () -> character_target;

	//	Note: no target gives no options.
	if (npc . points_to_object ())
	{
		assert (npc -> is_initialized ());
		assert (npc != Items :: Character :: get_player_character ());

		TiXmlElement * root = behavior -> RootElement ();
		assert (root != NULL);

		Reference <Strategies :: Behaviors :: AI> ai
			(npc -> get_active_state () . cast <Strategies :: Behaviors :: AI> ());

		load_player_options (root, ai);
	}
}

void Conversation_Engine ::
	load_player_options
	(
		const TiXmlElement * element,
		Reference <Strategies :: Behaviors :: AI> ai
	)
{
	assert (element != NULL);
	assert (ai . points_to_object ());
	assert (ai -> is_initialized ());
	
	if (element -> ValueStr () == "option")
	{
		Reference <Messages :: Message <Items :: Character> > message
		(
			new Messages :: Conversation_Message
			(
				Items :: Character :: get_player_character (),
				ai -> character,
				element
			)
		);

		call_observers (message);
	}
	
	if ((element -> ValueStr () == "reactions"))
	{
		return;
	}
	
	if ((element -> ValueStr () == "if_npc") && ! evaluate_npc_condition (element, ai))
	{
		return;
	}
	
	if ((element -> ValueStr () == "if_player")
		&& ! evaluate_player_character_condition (element))
	{
		return;
	}
	
	for (const TiXmlElement * sub_element = element -> FirstChildElement ();
		sub_element != NULL; sub_element = sub_element -> NextSiblingElement ())
	{
		load_player_options (sub_element, ai);
	}
}

bool Conversation_Engine ::
	evaluate_npc_condition
	(
		const TiXmlElement * element,
		Reference <Strategies :: Behaviors :: AI> ai
	)
{
	assert (is_initialized ());
	assert (element != NULL);
	assert (ai . points_to_object ());
	assert (ai -> is_initialized ());
	
	bool result = true;

	for (const TiXmlAttribute * attribute = element -> FirstAttribute (); attribute != NULL;
		attribute = attribute -> Next ())
	{
		result = result && evaluate_npc_expression (attribute, ai);
	}

	return result;
}

bool Conversation_Engine ::
	evaluate_npc_expression
	(
		const TiXmlAttribute * attribute,
		Reference <Strategies :: Behaviors :: AI> ai
	)
{
	assert (is_initialized ());
	assert (attribute != NULL);
	assert (ai . points_to_object ());
	assert (ai -> is_initialized ());

	const string & name = attribute -> NameTStr ();
	const string & value = attribute -> ValueStr ();
	Log :: log (me) << "evaluate: " << name  << " ? " << value << endl;
	
	assert ((name == "action") || (name == "fears") || (name == "likes"));

	if (name == "action")
	{
		Log :: log (me) << "result: " << to_string (ai -> get_current_action_name () == value) << endl;
		return ai -> get_current_action_name () == value;
	}

	assert ((value . at (0) == '<') || (value . at (0) == '>'));
	bool larger = (value . at (0) == '>');
	string temp = value;
	temp . erase (temp . begin ());
	float numeric_value = to_float (temp);

	bool is_smaller;
	if (name == "likes")
	{
		is_smaller = (numeric_value < 0.5);
	}
	else if (name == "fears")
	{
		is_smaller = (numeric_value < 0.2);
	}
	else
	{
		is_smaller = (numeric_value < ai -> character -> get_skill (name));
	}

	Log :: log (me) << "result: " << to_string (is_smaller != larger) << endl;
	return (is_smaller != larger);
}

bool Conversation_Engine ::
	evaluate_player_character_condition (const TiXmlElement * element)
{
	assert (is_initialized ());
	assert (element != NULL);
	
	bool result = true;

	for (const TiXmlAttribute * attribute = element -> FirstAttribute (); attribute != NULL;
		attribute = attribute -> Next ())
	{
		result = result && evaluate_player_character_expression (attribute);
	}

	return result;
}

bool Conversation_Engine ::
	evaluate_player_character_expression (const TiXmlAttribute * attribute)
{
	assert (is_initialized ());
	assert (attribute != NULL);

	const string & name = attribute -> NameTStr ();
	const string & value = attribute -> ValueStr ();
	Log :: log (me) << "evaluate: " << name  << " ? " << value << endl;
	
	if ((name == "knows") || (name == "has"))
	{
		Log :: log (me) << "result: false (I don't know and I don't have.)" << endl;
		return false;
	}

	assert ((value . at (0) == '<') || (value . at (0) == '>'));
	
	bool larger = (value . at (0) == '>');
	string temp = value;
	temp . erase (temp . begin ());
	float numeric_value = to_float (temp);

	bool is_smaller = (numeric_value
		< Items :: Character :: get_player_character () -> get_skill (name));

	Log :: log (me) << "result: " << to_string (is_smaller != larger) << endl;
	return (is_smaller != larger);
}
