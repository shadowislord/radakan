#include <tinyxml.h>

#include "engines/log.hpp"
#include "items/characters/character.hpp"
#include "messages/conversation_message.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Messages;

//	static
string Conversation_Message ::
	get_class_name ()
{
	return "Conversation_Message";
}

//  constructor
Conversation_Message ::
	Conversation_Message
	(
		Reference <Items :: Characters :: Character> new_from,
		Reference <Items :: Characters :: Character> new_to,
		const TiXmlElement * new_option
	) :
	Object (create_name (new_option)),
	Message <Items :: Characters :: Character> ("Doesn't matter.", new_from, new_to),
	option (new_option)
{
	Engines :: Log :: trace (me, Conversation_Message :: get_class_name ());
	
	//	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
Conversation_Message ::
	~Conversation_Message ()
{
	Engines :: Log :: trace (me, Conversation_Message :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Conversation_Message ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	virtual
Reference <Message <Items :: Characters :: Character> > Conversation_Message ::
	copy ()
	const
{
	return Reference <Message <Items :: Characters :: Character> >
		(new Conversation_Message (from, to, option));
}

Reference <Conversation_Message> Conversation_Message ::
	get_reaction ()
	const
{
	const TiXmlNode * temp = option;

	for (int i = 0; temp -> ValueStr () != "dialog"; i ++)
	{
		assert (i < 5);

		temp = temp -> Parent ();
		assert (temp != NULL);
	}

	temp = temp -> FirstChild ("reactions");
	assert (temp != NULL);

	for (int i = 0; temp -> ValueStr () != "reaction"; i ++)
	{
		assert (i < 5);

		temp = temp -> FirstChild ();
		assert (temp != NULL);
	}

	const TiXmlElement * reaction = temp -> ToElement ();
	assert (reaction != NULL);

	return Reference <Conversation_Message> (new Conversation_Message (to, from, reaction));
}

//	private & static
string Conversation_Message ::
	create_name (const TiXmlElement * option)
{
	assert (option != NULL);
	
	string result;
	
	if (option -> Attribute ("effect") != NULL)
	{
		result = "(" + * option -> Attribute (string ("effect")) + ") ";
	}
	
	if (option -> Attribute ("say") != NULL)
	{
		if (option -> Attribute ("shout") != NULL)
		{
			result += "Shout ";
		}
		result += "\"" + * option -> Attribute (string ("say")) + "\" ";
	}
	else if (option -> Attribute ("do") != NULL)
	{
		result += * option -> Attribute (string ("do"));
	}

	assert (! result . empty ());

	return result;
}
