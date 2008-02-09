#include "engines/input/mouse_data.hpp"
#include "engines/input/mouse_reader.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;
using namespace Radakan :: Engines :: Input;

//	static
string Mouse_Reader ::
	get_class_name ()
{
	return "Engines :: Input :: Mouse_Reader";
}

//	static
string Mouse_Reader ::
	get_left_mouse_button ()
{
	return Mouse_Data :: left_mouse_button;
}

//	static
string Mouse_Reader ::
	get_middle_mouse_button ()
{
	return Mouse_Data :: middle_mouse_button;
}

//	static
string Mouse_Reader ::
	get_right_mouse_button ()
{
	return Mouse_Data :: right_mouse_button;
}

Mouse_Reader ::
	Mouse_Reader () :
	Object (to_lower_case (get_class_name ()), "singleton")
{
	Engines :: Log :: trace (me, Mouse_Reader :: get_class_name ());

	assert (is_initialized ());
}

Mouse_Reader ::
	~Mouse_Reader ()
{
	Engines :: Log :: trace (me, Mouse_Reader :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Mouse_Reader ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}

bool Mouse_Reader ::
	is_mouse_button_pressed (string button, bool reset)
{
	assert (is_initialized ());

	if (0 < Mouse_Data :: get () -> pressed_mouse_buttons . count (button))
	{
		string to_be = "is";
		if (reset)
		{
			Mouse_Data :: get () -> pressed_mouse_buttons . erase (button);
			to_be = "was";
		}
		Engines :: Log :: log (me) << "Mouse button '" << button << "' " << to_be
			<< " pressed." << endl;
			
		return true;
	}
	return false;
}

const Mathematics :: Vector_3D & Mouse_Reader ::
	get_absolute_mouse_position ()
	const
{
	assert (is_initialized ());

	return Mouse_Data :: get () -> absolute_mouse_position;
}

const Mathematics :: Vector_3D & Mouse_Reader ::
	get_relative_mouse_position ()
	const
{
	assert (is_initialized ());

	return Mouse_Data :: get () -> relative_mouse_position;
}
