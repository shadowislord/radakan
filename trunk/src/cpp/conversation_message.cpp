#include "log.hpp"
#include "conversation_message.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Messages;

//	static
const string Conversation_Message ::
	get_class_name ()
{
	return "Conversation_Message";
}

//  constructor
Conversation_Message ::
	Conversation_Message
	(
		const TiXmlElement & new_option,
		Items :: Character & new_from,
		Items :: Character & new_to
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

//	static
string Conversation_Message ::
	create_name (const TiXmlElement & option)
{
	string result;
	
	if (option . Attribute ("shout") != NULL)
	{
		result = "Shout ";
	}
	if (option . Attribute ("say") != NULL)
	{
		result += "\"" + * option . Attribute (string ("say")) + "\" ";
	}
	if (option . Attribute ("action") != NULL)
	{
		result += * option . Attribute (string ("action")) + " ";
	}
	if (option . Attribute ("effect") != NULL)
	{
		result += "<" + * option . Attribute (string ("effect")) + "> ";
	}

	assert (! result . empty ());

	return result;
}
