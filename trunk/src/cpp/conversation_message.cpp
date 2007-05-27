#include "log.hpp"
#include "conversation_message.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Messages;

//	static
const string Conversation_Message ::
	get_class_name ()
{
	return "Conversation_Message";
}

//  constructor
Conversation_Message ::
	Conversation_Message (const string & information, Items :: Character & new_from, Items :: Character & new_to) :
	Object (information),
	from (new_from),
	to (new_to)
{
	Engines :: Log :: trace <Conversation_Message > (me);
	
	//	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
Conversation_Message ::
	~Conversation_Message ()
{
	Engines :: Log :: trace <Conversation_Message > (me, "~");
	assert (is_initialized ());

	//	Do nothing.
}

//	virtual
bool Conversation_Message ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}
