#include "engines/log.hpp"
#include "engines/input/mouse_data.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;
using namespace Radakan :: Engines :: Input;

//	static
string Mouse_Data ::
	get_class_name ()
{
	return "Engines :: Input :: Mouse_Data";
}

//	static
bool Mouse_Data ::
	is_mouse_button (string button)
{
	return (button == left_mouse_button)
		|| (button == middle_mouse_button)
		|| (button == right_mouse_button);
}

//	static
const string Mouse_Data ::
	left_mouse_button ("left");

//	static
const string Mouse_Data ::
	middle_mouse_button ("middle");

//	static
const string Mouse_Data ::
	right_mouse_button ("right");

//  constructor
Mouse_Data ::
	Mouse_Data () :
	Object (to_lower_case (get_class_name ()))
{
	Engines :: Log :: trace (me, Mouse_Data :: get_class_name ());

	assert (Mouse_Data :: is_initialized ());
}

//  destructor
Mouse_Data ::
	~Mouse_Data ()
{
	Engines :: Log :: trace (me, Mouse_Data :: get_class_name (), "~");
	assert (Mouse_Data :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Mouse_Data ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	assert (left_mouse_button != middle_mouse_button);
	assert (right_mouse_button != left_mouse_button);
	assert (middle_mouse_button != right_mouse_button);

	return true;
}
