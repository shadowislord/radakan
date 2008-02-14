#include "engines/log.hpp"
#include "items/character.hpp"
#include "messages/communications/converse.hpp"
#include "messages/communications/fight.hpp"
#include "set.hpp"
#include "strategies/actions/conversing.hpp"
#include "strategies/actions/fighting.hpp"
#include "strategies/behaviors/ai.hpp"

#include <tinyxml.h>

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Behaviors;

//	static
string AI ::
	get_class_name ()
{
	return "Strategies :: Behaviors :: AI";
}

//	static
boost :: scoped_ptr <map <unsigned int, TiXmlDocument> > AI ::
	all_behavior_files (new map <unsigned int, TiXmlDocument> ());

//  constructor
AI ::
	AI (Reference <Items :: Character> new_character) :
	Object (new_character . get_name () + "'s AI"),
	Behavior (new_character),
	calm (1, 0, 1),
	sensory_buffer
	(
		new Set <Messages :: Communications :: Communication>
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
	//	assert (Strategy <Behavior> :: is_initialized ());

	return true;
}

//	virtual
void AI ::
	call (Reference <Messages :: Communications :: Communication> message)
{
	//	We'll store a copy of the message for now, and process it on the next 'update'.
	//	We can't store the message itself, because that's const.
	sensory_buffer -> add (message);
}

//	virtual
Reference <Behavior> AI ::
	transit (Reference <Messages :: Nothing>)
{
	assert (is_initialized ());
	
	Pointer <Messages :: Communications :: Communication> actual_message;

	if (! sensory_buffer -> is_empty ())
	{
		actual_message = sensory_buffer -> get_child ();

		sensory_buffer -> drop (actual_message);
	}

/*	character -> get_body () -> move (0);
	character -> get_body () -> turn (0);*/

	if (actual_message . is_castable <Messages :: Communications :: Fight> ())
	{
		set_action (Actions :: Fighting :: get_class_name ());
	}
	else if (actual_message . is_castable <Messages :: Communications :: Converse> ())
	{
		if (calm > 0.5)	//	'listening' state:
		{
			set_action (Actions :: Conversing :: get_class_name ());
		}
		else
		{
			Engines :: Log :: show (character . get_name (true) + " is not listening.");
		}
	}

	Strategy_State_Machine
		<Actions :: Action, Messages :: Communications :: Communication> :: run
			(actual_message);

	return Reference <Behavior> (this);
}

bool AI ::
	evaluate_condition (const TiXmlElement * element)
{
	assert (is_initialized ());
	assert (element != NULL);

	bool result = true;

	for (const TiXmlAttribute * attribute = element -> FirstAttribute (); attribute != NULL;
		attribute = attribute -> Next ())
	{
		result = result && evaluate_expression (attribute);
	}

	return result;
}

bool AI ::
	evaluate_expression (const TiXmlAttribute * attribute)
{
	assert (is_initialized ());
	assert (attribute != NULL);

	const string & name = attribute -> NameTStr ();
	const string & value = attribute -> ValueStr ();
	Engines :: Log :: log (me) << "evaluate: " << name  << " ? " << value << endl;
	
	assert ((name == "action") || (name == "fears_player") || (name == "likes_player"));

	if (name == "action")
	{
		Engines :: Log :: log (me) << "result: "
			<< to_string (get_current_action_name () == value) << endl;
		
		return get_current_action_name () == value;
	}

	assert ((value . at (0) == '<') || (value . at (0) == '>'));
	bool larger = (value . at (0) == '>');
	string temp = value;
	temp . erase (temp . begin ());
	float numeric_value = to_float (temp);

	bool is_smaller;
	if (name == "fears_player")
	{
		is_smaller = (numeric_value < 0.2);
	}
	else if (name == "likes_player")
	{
		is_smaller = (numeric_value < 0.5);
	}
	else
	{
		is_smaller = (numeric_value < character -> get_skill (name));
	}

	Engines :: Log :: log (me) << "result: " << to_string (is_smaller != larger) << endl;
	return (is_smaller != larger);
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
		if (action_name == Actions :: Conversing :: get_class_name ())
		{
			set_active_state (Reference <Actions :: Action>
				(new Actions :: Conversing (character, Reference <AI> (this))));
		}
		else if (action_name == Actions :: Fighting :: get_class_name ())
		{
			set_active_state (Reference <Actions :: Action>
				(new Actions :: Fighting (character, Reference <AI> (this))));
		}
		else
		{
			assert (action_name == "none");
			
			set_active_state (Reference <Actions :: Action> ());
		}
	}
}

const boost :: shared_ptr <set <TiXmlDocument> > AI ::
	get_behavior_files ()
	const
{
	boost :: shared_ptr <set <TiXmlDocument> > result (new set <TiXmlDocument> ());

//	TODO	result -> insert ();

	return result;
}
