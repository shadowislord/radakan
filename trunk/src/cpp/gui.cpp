#include "gui.hpp"
#include <CEGUIExceptions.h>

using namespace std;
using namespace TSL;

//	static
const string GUI ::
	class_name ("GUI");

GUI ::
	GUI
	(
		string new_name,
		CEGUI :: Window & new_root
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
			error () << "Unknown CEGUI exception." << endl;
			abort ();
		}
	}

	GUI_Listener :: get () . subscribe (root_window);

	assert (is_initialized ());
}

GUI ::
	~GUI ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
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

//	virtual
void GUI ::
	call (const string & type, const string & message)
{
	assert (is_initialized ());
	assert (type == "show");
	assert (! message . empty ());

	text_window -> setText (message);
}
