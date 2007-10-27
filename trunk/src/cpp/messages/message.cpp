#include "engines/log.hpp"
#include "messages/message.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Messages;

//	static
template <class T> string Message <T> ::
	get_class_name ()
{
	return "Message <" + T :: get_class_name () + ">";
}

template <class T> const Reference <Message <T> > Message <T> ::
	update (new Message <T> ("update (static)"));
	
template <class T> const Reference <Message <T> > Message <T> ::
	terminate (new Message <T> ("terminate (static)"));

//  constructor
template <class T> Message <T> ::
	Message (string new_name, Reference <T> new_from, Reference <T> new_to) :
	Object (new_name),
	from (new_from),
	to (new_to)
{
	Engines :: Log :: trace
		(me, Message <T> :: get_class_name (), new_name, from . get_name (), to . get_name ());
	assert (Object :: is_initialized ());
	assert (from . is_initialized ());
	assert (to . is_initialized ());
	
	//	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
template <class T> Message <T> ::
	~Message ()
{
	Engines :: Log :: trace (me, Message <T> :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
template <class T> bool Message <T> ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

#include "engines/game.hpp"
#include "items/character.hpp"

template class Message <Engines :: Game>;
template class Message <Items :: Character>;
