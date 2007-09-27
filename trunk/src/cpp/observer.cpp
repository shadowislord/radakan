#include "observer.hpp"
#include "log.hpp"

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
#include "character.hpp"
#include "gui.hpp"
#include "log.hpp"
#include "play_state.hpp"

template class Observer <Engines :: Log>;
template class Observer <GUI>;
template class Observer <Items :: Character>;
template class Observer <Strategies :: Play_State>;
