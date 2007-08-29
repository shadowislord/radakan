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
	Object (new_option . Attribute ("say")),
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
