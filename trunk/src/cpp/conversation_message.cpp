#include <tinyxml.h>

#include "character.hpp"
#include "conversation_message.hpp"
#include "log.hpp"

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
		boost :: shared_ptr <const TiXmlElement> new_option,
		Reference <Items :: Character> new_from,
		Reference <Items :: Character> new_to
	) :
	Object (create_name (new_option)),
	option (new_option),
	from (new_from),
	to (new_to)
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

Reference <Conversation_Message> Conversation_Message ::
	get_reaction ()
	const
{
	const TiXmlNode * temp = option . get ();

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

	boost :: shared_ptr <const TiXmlElement> reaction (temp -> ToElement ());
	assert (reaction);

	return Reference <Conversation_Message> (new Conversation_Message (reaction, to, from));
}

//	private & static
string Conversation_Message ::
	create_name (boost :: shared_ptr <const TiXmlElement> option)
{
	string result;
	
	if (option -> Attribute ("shout") != NULL)
	{
		result = "Shout ";
	}
	if (option -> Attribute ("say") != NULL)
	{
		result += "\"" + * option -> Attribute (string ("say")) + "\" ";
	}
	if (option -> Attribute ("action") != NULL)
	{
		result += * option -> Attribute (string ("action")) + " ";
	}
	if (option -> Attribute ("effect") != NULL)
	{
		result += "<" + * option -> Attribute (string ("effect")) + "> ";
	}

	assert (! result . empty ());

	return result;
}
