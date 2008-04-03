#include "engines/log.hpp"
#include "items/character.hpp"
#include "messages/communications/communication.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Messages;
using namespace Radakan :: Messages :: Communications;

//	static
string Communication ::
	get_class_name ()
{
	return "Messages :: Communications :: Communication";
}

//  constructor
Communication ::
	Communication
	(
		string new_name,
		Reference <Items :: Character> new_from,
		Reference <Items :: Character> new_to
	) :
	Object (new_name),
	from (new_from),
	to (new_to)
{
	Engines :: Log :: trace
		(me, Communication :: get_class_name (), from . get_name (), to . get_name ());
	assert (Object :: is_initialized ());
	
	//	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
Communication ::
	~Communication ()
{
	Engines :: Log :: trace (me, Communication :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Communication ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}
