#include "gui.hpp"
#include <CEGUIExceptions.h>
#include <elements/CEGUIPushButton.h>

using namespace std;
using namespace TSL;

//	static
const string GUI ::
	get_class_name ()
{
	return "GUI";
}

GUI ::
	GUI
	(
		string new_name,
		CEGUI :: Window & new_root
	) :
	Object (new_name),
	root_window (new_root),
	subscriber (& GUI :: handle_event, this)
{
	try
	{
		text_window = root_window . getChild ("sector-text");
	}
	catch (CEGUI :: UnknownObjectException & exception)
	{
		try
		{
			text_window = root_window . getChild ("menu-text");
		}
		catch (CEGUI :: UnknownObjectException & exception)
		{
			Engines :: Log :: error (me) << "Unknown CEGUI exception." << endl;
			abort ();
		}
	}

	subscribe (root_window);

	Engines :: Log :: get () . register_observer (* this);

	assert (is_initialized ());
}

GUI ::
	~GUI ()
{
	Engines :: Log :: trace (me, GUI :: get_class_name (), "~");
	assert (is_initialized ());

	forget_dependencies ();
}

//	virtual
bool GUI ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	return text_window != NULL;
}

//	virtual
void GUI ::
	call (const Object & message)
{
	assert (is_initialized ());

	text_window -> setText (message);
}

void GUI ::
	subscribe (CEGUI :: Window & window)
{
	assert (is_initialized ());
	CEGUI :: Window * temp;
	for (unsigned int i = 0; i < window . getChildCount (); i ++)
	{
		temp = window . getChildAtIdx (i);
		if (temp -> getType () . find ("Button") != string :: npos)
		{
			temp -> subscribeEvent (CEGUI :: PushButton :: EventClicked, subscriber);
		}
		else
		{
			subscribe (* temp);
		}
	}
}

bool GUI ::
	handle_event (const CEGUI :: EventArgs & arguments)
{
	Engines :: Log :: trace (me, GUI :: get_class_name (), "handle_event", "~arguments~");
	assert (is_initialized ());

	CEGUI :: WindowEventArgs * window_event_arguments = (CEGUI :: WindowEventArgs *)(& arguments);
	if (window_event_arguments == NULL)
	{
		Engines :: Log :: show ("Unknown event type...");
	}
	else
	{
		string caption (window_event_arguments -> window -> getText () . c_str ());

		Object * message = new Object (caption);
		call_observers (* message);
		delete message;
	}

	return true;
}
