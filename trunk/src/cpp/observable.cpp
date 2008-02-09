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
	Observable () :
	Object ("The name doesn't matter as this class is an abstact class."),
	observers (new Set <Observer <T> > (me . get_name (true) + "'s observers", Container <Observer <T> > :: unlimited (), true))
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

//	to avert linking errors:
#include "messages/button_event.hpp"
#include "messages/communications/communication.hpp"
#include "messages/list_event.hpp"
#include "messages/list_update.hpp"
#include "messages/nothing.hpp"

template class Observable <Messages :: Button_Event>;
template class Observable <Messages :: Communications :: Communication>;
template class Observable <Messages :: List_Event>;
template class Observable <Messages :: List_Update>;
template class Observable <Messages :: Nothing>;
