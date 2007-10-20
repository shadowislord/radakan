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
		
		chat_window -> setSortingEnabled (true);
	}
	catch (CEGUI :: UnknownObjectException & exception)
	{
		//	We don't have to do anything.
	}

	subscribe (* root_window . get ());

	#ifdef RADAKAN_DEBUG
		Engines :: Log :: get () -> register_observer (Reference <Observer <Engines :: Log> > (this));
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
	assert (is_initialized ());
	Engines :: Log :: trace (me, GUI :: get_class_name (), "call", message . get_name ());

	if (message . is_castable <Set <Messages :: Conversation_Message> > ())
	{
		Engines :: Log :: log (me) << "Message set detected." << endl;
		
		assert (chat_window);

		//	If there are old options, remove them.
		if (messages . points_to_object ())
		{
			message_map . clear ();
			chat_window -> resetList ();
		}
		
		messages = (const_cast <Reference <Object> &> (message)) . cast <Set <Messages :: Conversation_Message> > ();
		assert (messages . is_initialized ());
		assert (messages . points_to_object ());
		assert (messages -> is_initialized ());
		assert (messages -> get_child () . points_to_object ());
		
		for (Reference <Messages :: Conversation_Message> option = messages -> get_child ();
			option . points_to_object (); option = messages -> get_another_child ())
		{
			CEGUI :: ListboxTextItem * item = new CEGUI :: ListboxTextItem (option -> name);
			message_map [item] = option;

			//	As the chat window is sorted, the item is added in the right place.
			chat_window -> addItem (item);

			Engines :: Log :: log (me) << "Option '" << option -> name << "' added." << endl;
		}
		
		for (int i = 0; i < chat_window -> getItemCount (); i ++)
		{
			chat_window -> ensureItemIsVisible (i);
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

			Reference <Messages :: Conversation_Message> message
				= message_map [chat_window -> getFirstSelectedItem ()];
			assert (message . points_to_object ());
			assert (message -> is_initialized ());

			chat_window -> clearAllSelections ();

			call_observers (message);
		}
		else	//	button
		{
			string caption (window_event_arguments -> window -> getText () . c_str ());

			Reference <Object> message (new Object (caption));

			call_observers (message);
		}
	}

	return true;
}
