#include "engines/log.hpp"
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
string Mediator_Implementation ::
	get_class_name ()
{
	return "Engines :: Mediator_Implementation";
}

//  constructor
Mediator_Implementation ::
	Mediator_Implementation () :
	Object ("mediator implementation", "singleton")
{
	Engines :: Log :: trace (me, Mediator_Implementation :: get_class_name ());

	assert (is_initialized ());
}

//  destructor
Mediator_Implementation ::
	~Mediator_Implementation ()
{
	Engines :: Log :: trace (me, Mediator_Implementation :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Mediator_Implementation ::
	is_initialized ()
	const
{
	assert (Singleton <Mediator_Implementation> :: is_initialized ());

	return true;
}
