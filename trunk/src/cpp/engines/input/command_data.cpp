#include "engines/log.hpp"
#include "engines/input/command_data.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;
using namespace Radakan :: Engines :: Input;

//	static
string Command_Data ::
	get_class_name ()
{
	return "Engines :: Input :: Command_Data";
}

//  constructor
Command_Data ::
	Command_Data () :
	Object (to_lower_case (get_class_name ()), true)
{
	Log :: trace (me, Command_Data :: get_class_name ());

	assert (Command_Data :: is_initialized ());
}

//  destructor
Command_Data ::
	~Command_Data ()
{
	Log :: trace (me, Command_Data :: get_class_name (), "~");
	assert (Command_Data :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Command_Data ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}
