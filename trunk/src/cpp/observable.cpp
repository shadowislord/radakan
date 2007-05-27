#include "log.hpp"
#include "observable.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Observable <T> ::
	get_class_name ()
{
	return "Observable <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Observable <T> ::
	Observable () :
	Object ("The name doesn't matter as this class is an abstact class."),
	//	Set a custom context to evade problems with the dependencies.
	observers (my + "observers", Set <Observer <T> > :: unlimited)
{
	Engines :: Log :: trace <Observable> (me, "");
	
	//	Do nothing.

	assert (Observable <T> :: is_initialized ());
}

//  destructor
template <class T> Observable <T> ::
	~Observable ()
{
	Engines :: Log :: trace <Observable> (me, "~");
	assert (Observable <T> :: is_initialized ());
	
	//	Do nothing.
}

//	virtual
template <class T> bool Observable <T> ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}

template <class T> void Observable <T> ::
	call_observers (const Object & message)
{
	assert (is_initialized ());
	assert (is_type <T> ());

	T & myself = to_type <T> ();

	for (Observer <T> * observer = observers . get_child (); observer != NULL;
		observer = observers . get_another_child ())
	{
		observer -> call (message);
	}
}

template <class T> void Observable <T> ::
	register_observer (Observer <T> & observer)
{
	assert (is_initialized ());
	
	observers . add (observer);
}

template <class T> void Observable <T> ::
	drop_observer (Observer <T> & observer)
{
	assert (is_initialized ());
	
	observers . drop (observer);
}

//	to avert linking errors:
#include "character.hpp"
#include "gui.hpp"
#include "log.hpp"
#include "play_state.hpp"

template class Observable <Algorithms :: Play_State>;
template class Observable <Engines :: Log>;
template class Observable <GUI>;
template class Observable <Items :: Character>;
