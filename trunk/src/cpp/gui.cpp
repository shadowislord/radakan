#include "gui.hpp"
#include <CEGUIExceptions.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxTextItem.h>
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
	CEGUI :: Window * temp;

	try
	{
		temp = root_window . getChild ("play-log");
	}
	catch (CEGUI :: UnknownObjectException & exception)
	{
		try
		{
			temp = root_window . getChild ("menu-log");
		}
		catch (CEGUI :: UnknownObjectException & exception)
		{
			Engines :: Log :: error (me) << "No log window found." << endl;
			abort ();
		}
	}
	
	#ifdef TSL_WIN
		try
		{
	#endif

		log_window = dynamic_cast <CEGUI :: Listbox *> (temp);

	#ifdef TSL_WIN
		}
		catch(std::__non_rtti_object e)
		{
			Engines :: Log :: error (me) << "Log window could not be dynamically cast. Falling back to unsafe casting." << endl;
			log_window = (CEGUI :: Listbox *) (temp);
		}
	#endif

	assert (log_window != NULL);

	subscribe (root_window);

	Engines :: Log :: get () . register_observer (* this);

	assert (is_initialized ());
}

GUI ::
	~GUI ()
{
	Engines :: Log :: trace (me, GUI :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool GUI ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	assert (log_window != NULL);

	return true;
}

//	virtual
void GUI ::
	call (const Object & message)
{
	assert (is_initialized ());

	CEGUI :: ListboxTextItem * item = new CEGUI :: ListboxTextItem (message . name);
	log_window -> addItem (item);
	log_window -> ensureItemIsVisible (log_window -> getItemCount ());
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

	// I had to declare this outside due to scoping.
	const CEGUI :: WindowEventArgs * window_event_arguments;

	#ifdef TSL_WIN
		try
		{
	#endif

	
		window_event_arguments = dynamic_cast <const CEGUI :: WindowEventArgs *> (& arguments);

	#ifdef TSL_WIN
		}
		catch(std::__non_rtti_object e)
		{
			Engines :: Log :: error (me) << "The window's arguments list could not be type casted. Fallig back to unsafe type casts." << endl;

			window_event_arguments
				= (const CEGUI :: WindowEventArgs *) (& arguments);
		}
	#endif


	if (window_event_arguments == NULL)
	{
		Engines :: Log :: show ("Unknown event type...");
	}
	else
	{
		string caption (window_event_arguments -> window -> getText () . c_str ());

		call_observers (caption);	//	Automatically construct an Object.
	}

	return true;
}
