#include "gui.hpp"
#include <CEGUIExceptions.h>

using namespace std;
using namespace tsl;

GUI ::
	GUI
	(
		string new_name,
		CEGUI :: Window & new_root,
		GUI_Listener & gui_listener
	) :
	Object (new_name),
	root_window (new_root)
{
	assert (Object :: is_initialized ());

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
			abort ();
		}
	}

	gui_listener . subscribe (root_window);

	assert (is_initialized ());
}

GUI ::
	~GUI ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;
	assert (is_initialized ());
}

//	virtual
bool GUI ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	return text_window != NULL;
}

//	static
string GUI ::
	get_class_name ()
{
	return "GUI";
}

void GUI ::
	update_message ()
{
	assert (is_initialized ());

	if (! message . str () . empty ())
	{
		log (debugging) << "Message: " << message . str () << endl;
	
		text_window -> setText (message . str ());

		//	flush the message:
		message . str ("");
	}
}
