#include "engines/log.hpp"
#include "observer.hpp"
#include "observable.hpp"
#include "pointer.hpp"
#include "set.hpp"
#include "slot.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> string Observable <T> ::
	get_class_name ()
{
	return "Observable <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Observable <T> ::
	Observable (int maximum_number_of_observers) :
	Object ("The name doesn't matter as this class is an abstact class."),
	//	Set a custom context to evade problems with the dependencies.
	observers
	(
		maximum_number_of_observers == 1
		? (Reference <Container <Observer <T> > > (new Slot <Observer <T> > (name + "'s observers")))
		: (Reference <Container <Observer <T> > > (new Set  <Observer <T> >
			(name + "'s observers", maximum_number_of_observers)))
	)
{
	Engines :: Log :: trace (me, Observable :: get_class_name (), "");
	
	//	Do nothing.

	assert (Observable <T> :: is_initialized ());
}

//  destructor
template <class T> Observable <T> ::
	~Observable ()
{
	Engines :: Log :: trace (me, Observable :: get_class_name (), "~");
	assert (Observable <T> :: is_initialized ());
	
	//	Do nothing.
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
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
	call_observers (const Reference <T> & message)
{
	assert (is_initialized ());

	for (Pointer <Observer <T> > observer = observers -> get_child ();
		observer . points_to_object (); observer = observers -> get_another_child ())
	{
		observer -> call (message);
	}
}

template <class T> void Observable <T> ::
	register_observer (Reference <Observer <T> > observer)
{
	Engines :: Log :: trace (me, Observable :: get_class_name (), "register_observer", observer . get_name ());
	assert (is_initialized ());
	
	bool check = observers -> add (observer);
	assert (check);
}

template <class T> void Observable <T> ::
	drop_observer (Reference <Observer <T> > observer)
{
	assert (is_initialized ());

	observers -> drop (observer);
}

//	to avert linking errors:
#include "items/character.hpp"
#include "messages/message.hpp"

template class Observable <Messages :: Message <Items :: Character> >;
template class Observable <Object>;
