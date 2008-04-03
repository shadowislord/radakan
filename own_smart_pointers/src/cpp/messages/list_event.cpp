#include "engines/log.hpp"
#include "messages/list_event.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Messages;

//	static
string List_Event ::
	get_class_name ()
{
	return "Messages :: List_Event";
}

//  constructor
List_Event ::
	List_Event (string new_name) :
	Object (new_name)
{
	Engines :: Log :: trace (me, List_Event :: get_class_name (), new_name);
	assert (Object :: is_initialized ());
	
	//	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
List_Event ::
	~List_Event ()
{
	Engines :: Log :: trace (me, List_Event :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool List_Event ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}
