#include "engines/log.hpp"
#include "engines/mediator.hpp"
#include "engines/mediator_implementation.hpp"
#include "messages/button_event.hpp"
#include "messages/communications/communication.hpp"
#include "messages/list_event.hpp"
#include "messages/list_update.hpp"
#include "messages/nothing.hpp"
#include "observable.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;

//	static
string Mediator ::
	get_class_name ()
{
	return "Engines :: Mediator";
}

//  constructor
Mediator ::
	Mediator () :
	Object ("mediator", true)	//	Here 'true' means 'prevent automatic destruction'.
{
	Engines :: Log :: trace (me, Mediator :: get_class_name ());

	new Mediator_Implementation ();

	assert (is_initialized ());
}

//  destructor
Mediator ::
	~Mediator ()
{
	Engines :: Log :: trace (me, Mediator :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Mediator ::
	is_initialized ()
	const
{
	assert (Singleton <Mediator> :: is_initialized ());

	return true;
}

template <class T> void Mediator ::
	call_observers (Reference <T> message)
{
	assert (Mediator :: is_initialized ());

	Engines :: Mediator_Implementation :: get ()
		-> Observable <T> :: call_observers (message);
}

template <class T> void Mediator ::
	register_observer (Reference <Observer <T> > observer)
{
	assert (Mediator :: is_initialized ());

	Engines :: Mediator_Implementation :: get ()
		-> Observable <T> :: register_observer (observer);
}

//	to avoid linking errors:
template void Mediator :: call_observers <Messages :: Button_Event>
	(Reference <Messages :: Button_Event> message);
template void Mediator :: call_observers <Messages :: Communications :: Communication>
	(Reference <Messages :: Communications :: Communication> message);
template void Mediator :: call_observers <Messages :: List_Event>
	(Reference <Messages :: List_Event> message);
template void Mediator :: call_observers <Messages :: List_Update>
	(Reference <Messages :: List_Update> message);
template void Mediator :: call_observers <Messages :: Nothing>
	(Reference <Messages :: Nothing> message);

template void Mediator :: register_observer <Messages :: Button_Event>
	(Reference <Observer <Messages :: Button_Event> > observer);
template void Mediator :: register_observer <Messages :: Communications :: Communication>
	(Reference <Observer <Messages :: Communications :: Communication> > observer);
template void Mediator :: register_observer <Messages :: List_Event>
	(Reference <Observer <Messages :: List_Event> > observer);
template void Mediator :: register_observer <Messages :: List_Update>
	(Reference <Observer <Messages :: List_Update> > observer);
template void Mediator :: register_observer <Messages :: Nothing>
	(Reference <Observer <Messages :: Nothing> > observer);
