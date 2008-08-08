#include "engines/log.hpp"
#include "messages/button_event.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Messages;

//	static
string Button_Event ::
	get_class_name ()
{
	return "Messages :: Button_Event";
}

//  constructor
Button_Event ::
	Button_Event (string new_name) :
	Object (new_name)
{
	Engines :: Log :: trace (me, Button_Event :: get_class_name (), new_name);
	assert (Object :: is_initialized ());
	
	//	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
Button_Event ::
	~Button_Event ()
{
	Engines :: Log :: trace (me, Button_Event :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Button_Event ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}
