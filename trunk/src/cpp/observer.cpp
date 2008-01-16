#include "engines/log.hpp"
#include "observer.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> string Observer <T> ::
	get_class_name ()
{
	return "Observer <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Observer <T> ::
	Observer () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Engines :: Log :: trace (me, Observer <T> :: get_class_name ());

	//	Do nothing.

	assert (Observer <T> :: is_initialized ());
}

//  destructor
template <class T> Observer <T> ::
	~Observer ()
{
	Engines :: Log :: trace (me, Observer <T> :: get_class_name (), "~");
	assert (Observer <T> :: is_initialized ());

	//	Do nothing.
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
}

//	virtual
template <class T> bool Observer <T> ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	to avert linking errors:
#include "items/characters/character.hpp"
#include "messages/message.hpp"

template class Observer <Messages :: Message <Items :: Characters :: Character> >;
template class Observer <Messages :: Message <Object> >;
template class Observer <Object>;
