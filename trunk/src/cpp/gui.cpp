#include <OgreCEGUIRenderer.h>

#include <CEGUIWindow.h>
#include <CEGUIExceptions.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxTextItem.h>
#include <elements/CEGUIPushButton.h>

#include "conversation_message.hpp"
#include "log.hpp"

#include "gui.hpp"

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

	//	We're re-using temp now.
	try
	{
		temp = root_window -> getChild ("play-chat");
		assert (temp != NULL);

		#ifdef RADAKAN_TARIQWALJI
			try
			{
		#endif
			chat_window . reset (dynamic_cast <CEGUI :: Listbox *> (temp));

		#ifdef RADAKAN_TARIQWALJI
			}
			catch (__non_rtti_object e)
			{
				Engines :: Log :: error (me) << "Chat window could not be dynamically cast. Falling back to unsafe type casting." << endl;
				chat_window . reset ((CEGUI :: Listbox *) (temp));
			}
		#endif
	}
	catch (CEGUI :: UnknownObjectException & exception)
	{
		//	We don't have to do anything.
	}

	subscribe (root_window);

	Engines :: Log :: get () -> register_observer (Reference <Observer <Engines :: Log> > (this));

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
	call (Reference <const Object> message)
{
	assert (is_initialized ());

	if (message -> is_type <Set <Messages :: Conversation_Message> > ())
	{
		assert (chat_window);

		if (! messages . points_to_object ())
		{
			chat_window -> resetList ();	//	Clear the list.

			messages . reset_pointee ();
		}
		
		messages = message -> to_type <Set <Messages :: Conversation_Message> > ();
		
		for (Reference <Messages :: Conversation_Message> option = messages -> get_child ();
					! option . points_to_object (); option = messages -> get_another_child ())
		{
			boost :: shared_ptr <CEGUI :: ListboxTextItem> item (new CEGUI :: ListboxTextItem (option -> name));
			item -> setUserData ((void *) (& option));
			
			chat_window -> addItem (item . get ());
			chat_window -> ensureItemIsVisible (chat_window -> getItemCount ());
		}
	}
	else	//	log messages
	{
		CEGUI :: ListboxTextItem * item = new CEGUI :: ListboxTextItem (message -> name);
		log_window -> addItem (item);
		log_window -> ensureItemIsVisible (log_window -> getItemCount ());
	}
}

void GUI ::
	subscribe (boost :: shared_ptr <CEGUI :: Window> window)
{
	assert (is_initialized ());
	boost :: shared_ptr <CEGUI :: Window> temp;
	for (unsigned int i = 0; i < window -> getChildCount (); i ++)
	{
		temp . reset (window -> getChildAtIdx (i));
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
			subscribe (temp);
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

	#ifdef RADAKAN_TARIQWALJI
		}
		catch (__non_rtti_object e)
		{
			Engines :: Log :: error (me) << "The window's arguments list could not be type casted. Fallig back to unsafe type casts." << endl;

			window_event_arguments
				= . reset ((const CEGUI :: WindowEventArgs *) (& arguments));
		}
	#endif

	if (! window_event_arguments)
	{
		Engines :: Log :: show ("Unknown event type...");
	}
	else
	{
		if (window_event_arguments -> window -> getType () . find ("Listbox") != string :: npos)
		{
			assert (chat_window);

			if (chat_window -> getSelectedCount () == 0)
			{
				return true;
			}
			
			Reference <Object> message (* (Reference <Object> *) (chat_window -> getFirstSelectedItem () -> getUserData ()));
			assert (message . points_to_object ());
			assert (message -> is_initialized ());

			chat_window -> clearAllSelections ();

			call_observers (message);
		}
		else	//	button
		{
			string caption (window_event_arguments -> window -> getText () . c_str ());

			Reference <const Object> message (new Object (caption));

			call_observers (message);
		}
	}

	return true;
}
