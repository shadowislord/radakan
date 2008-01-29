#include "engines/input/command_data.hpp"
#include "engines/input/command_reader.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;
using namespace Radakan :: Engines :: Input;

//	static
string Command_Reader ::
	get_class_name ()
{
	return "Engines :: Input :: Command_Reader";
}

Command_Reader ::
	Command_Reader () :

	//	Here 'true' means 'prevent automatic destruction'.
	Object (to_lower_case (get_class_name ()), true)	
{
	Engines :: Log :: trace (me, Command_Reader :: get_class_name ());

	assert (is_initialized ());
}

Command_Reader ::
	~Command_Reader ()
{
	Engines :: Log :: trace (me, Command_Reader :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Command_Reader ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}

string Command_Reader ::
	get_communication_option ()
{
	assert (is_initialized ());

	return Command_Data :: get () -> selected_communication_option;
}

bool Command_Reader ::
	has_command (Reference <Object> caller, string command_name, bool reset)
{
	Engines :: Log :: trace (me, Command_Reader :: get_class_name (), "has_command",
		caller . get_name (), command_name, bool_to_string (reset));
	assert (is_initialized ());

	if (Command_Data :: get () -> clicked_button == command_name)
	{
		if (reset)
		{
			Command_Data :: get () -> clicked_button = "";
		}
	}
	else
	{
		//	Is there a key shortcut for the command?
		string key_binding = Settings :: get () -> get_key_name
			(caller . get_name (true), command_name);
		if (0 == Command_Data :: get () -> pressed_keys . count (key_binding))
		{
			return false;
		}

		if (reset)
		{
			Command_Data :: get () -> pressed_keys . erase (key_binding);
		}
	}
	
	return true;
}
