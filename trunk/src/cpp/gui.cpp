#include <OgreCEGUIRenderer.h>

#include <CEGUIWindow.h>
#include <CEGUIExceptions.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxTextItem.h>
#include <elements/CEGUIPushButton.h>

#include "engines/log.hpp"
#include "gui.hpp"
#include "messages/message.hpp"

using namespace std;
using namespace Radakan;

//	static
string GUI ::
	get_class_name ()
{
	return "GUI";
}

GUI ::
	GUI
	(
		string new_name,
		boost :: shared_ptr <CEGUI :: Window> new_root
	) :
	Object (new_name),
	root_window (new_root),
	subscriber (new CEGUI :: SubscriberSlot (& GUI :: handle_event, this))
{
	CEGUI :: Window * temp;

	try
	{
		temp = root_window -> getChild ("play-log");
	}
	catch (CEGUI :: UnknownObjectException & exception)
	{
		try
		{
			temp = root_window -> getChild ("menu-log");
		}
		catch (CEGUI :: UnknownObjectException & exception)
		{
			Engines :: Log :: error (me) << "No log window found." << endl;
			abort ();
		}
	}
	
	#ifdef RADAKAN_TARIQWALJI
		try
		{
	#endif

		log_window . reset (dynamic_cast <CEGUI :: Listbox *> (temp));

	#ifdef RADAKAN_TARIQWALJI
		}
		catch (__non_rtti_object e)
		{
			Engines :: Log :: error (me) << "Log window could not be dynamically cast. Falling back to unsafe casting." << endl;
			log_window . reset ((CEGUI :: Listbox *) (temp));
		}
	#endif
	
	assert (log_window);	//	Make sure it doesn't point to NULL.


	subscribe (* root_window . get ());

	#ifdef RADAKAN_DEBUG
		Engines :: Log :: get () -> register_observer (Reference <Observer <Object> > (this));
	#endif

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
	assert (log_window);

	return true;
}

//	virtual
void GUI ::
	call (const Reference <Object> & message)
{
	Engines :: Log :: trace (me, GUI :: get_class_name (), "call", message . get_name ());
	assert (is_initialized ());
	assert (message . is_initialized ());
	assert (message . points_to_object ());
	assert (message -> is_initialized ());

	CEGUI :: ListboxTextItem * item = new CEGUI :: ListboxTextItem (message . get_name ());
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
			temp -> subscribeEvent (CEGUI :: PushButton :: EventClicked, * subscriber);
		}
		else if (temp -> getType () . find ("Listbox") != string :: npos)
		{
			temp -> subscribeEvent (CEGUI :: Listbox :: EventSelectionChanged, * subscriber);
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
	boost :: scoped_ptr <const CEGUI :: WindowEventArgs> window_event_arguments;

	#ifdef RADAKAN_TARIQWALJI
		try
		{
	#endif

		window_event_arguments . reset (dynamic_cast <const CEGUI :: WindowEventArgs *> (& arguments));
		assert (window_event_arguments);

	#ifdef RADAKAN_TARIQWALJI
		}
		catch (__non_rtti_object e)
		{
			Engines :: Log :: error (me) << "The window's arguments list could not be type casted. Fallig back to unsafe type casts." << endl;

			window_event_arguments
				= . reset ((const CEGUI :: WindowEventArgs *) (& arguments));
		}
	#endif

	string caption (window_event_arguments -> window -> getText () . c_str ());

	call_observers (Reference <Object> (new Object (caption)));

	return true;
}
