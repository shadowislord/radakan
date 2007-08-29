#include "gui.hpp"
#include <CEGUIExceptions.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxTextItem.h>
#include <elements/CEGUIPushButton.h>

using namespace std;
using namespace Radakan;

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
	subscriber (& GUI :: handle_event, this),
	messages (NULL)
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
	
	#ifdef RADAKAN_TARIQWALJI
		try
		{
	#endif

		log_window = dynamic_cast <CEGUI :: Listbox *> (temp);

	#ifdef RADAKAN_TARIQWALJI
		}
		catch(std::__non_rtti_object e)
		{
			Engines :: Log :: error (me) << "Log window could not be dynamically cast. Falling back to unsafe casting." << endl;
			log_window = (CEGUI :: Listbox *) (temp);
		}
	#endif
	
	assert (log_window != NULL);

	//	We're re-using temp now.
	try
	{
		temp = root_window . getChild ("play-chat");
		assert (temp != NULL);

		#ifdef RADAKAN_TARIQWALJI
			try
			{
		#endif
			chat_window = dynamic_cast <CEGUI :: Listbox *> (temp);

		#ifdef RADAKAN_TARIQWALJI
			}
			catch (__non_rtti_object e)
			{
				Engines :: Log :: error (me) << "Chat window could not be dynamically cast. Falling back to unsafe type casting." << endl;
				chat_window = (CEGUI :: Listbox *) (temp);
			}
		#endif
	}
	catch (CEGUI :: UnknownObjectException & exception)
	{
		//	This is no problem.
		chat_window = NULL;
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

	prepare_for_destruction ();

	if (messages != NULL)
	{
		delete messages;
	}
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

	if (message . is_type <Set <Messages :: Conversation_Message> > ())
	{
		assert (chat_window != NULL);

		if (messages != NULL)
		{
			chat_window -> resetList ();	//	Clear the list.

			delete messages;
		}
		
		messages = & message . to_type <Set <Messages :: Conversation_Message> > ();
		
		for (Messages :: Conversation_Message * option = messages -> get_child ();
					option != NULL; option = messages -> get_another_child ())
		{
			CEGUI :: ListboxTextItem * item = new CEGUI :: ListboxTextItem (option -> name);
			item -> setUserData (option);
			
			chat_window -> addItem (item);
			chat_window -> ensureItemIsVisible (chat_window -> getItemCount ());
		}
	}
	else	//	log messages
	{
		CEGUI :: ListboxTextItem * item = new CEGUI :: ListboxTextItem (message . name);
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
			temp -> subscribeEvent (CEGUI :: PushButton :: EventClicked, subscriber);
		}
		else if (temp -> getType () . find ("Listbox") != string :: npos)
		{
			temp -> subscribeEvent (CEGUI :: Listbox :: EventSelectionChanged, subscriber);
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

	#ifdef RADAKAN_TARIQWALJI
		try
		{
	#endif

		window_event_arguments = dynamic_cast <const CEGUI :: WindowEventArgs *> (& arguments);

	#ifdef RADAKAN_TARIQWALJI
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
		if (window_event_arguments -> window -> getType () . find ("Listbox") != string :: npos)
		{
			assert (chat_window != NULL);

			if (chat_window -> getSelectedCount () == 0)
			{
				return true;
			}
			
			Object * message = (Object *) (chat_window -> getFirstSelectedItem () -> getUserData ());
			assert (message != NULL);
			assert (message -> is_initialized ());

			chat_window -> clearAllSelections ();

			call_observers (* message);
		}
		else	//	button
		{
			string caption (window_event_arguments -> window -> getText () . c_str ());

			call_observers (caption);	//	Automatically construct an Object.
		}
	}

	return true;
}
