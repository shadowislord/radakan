#include <OgreCEGUIRenderer.h>

#include <CEGUIWindow.h>
#include <CEGUIExceptions.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxTextItem.h>
#include <elements/CEGUIPushButton.h>

#include "engines/log.hpp"
#include "messages/conversation_message.hpp"
#include "play_state_gui.hpp"

using namespace std;
using namespace Radakan;

//	static
string Play_State_GUI ::
	get_class_name ()
{
	return "Play_State_GUI";
}

Play_State_GUI ::
	Play_State_GUI
	(
		string new_name,
		boost :: shared_ptr <CEGUI :: Window> new_root
	) :
	Object (new_name),
	GUI ("Doesn't matter", new_root)
{
	CEGUI :: Window * temp = root_window -> getChild ("play-chat");
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
	
	assert (is_initialized ());
}

Play_State_GUI ::
	~Play_State_GUI ()
{
	Engines :: Log :: trace (me, Play_State_GUI :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Play_State_GUI ::
	is_initialized ()
	const
{
	assert (GUI :: is_initialized ());
	assert (chat_window);

	return true;
}

//	virtual
void Play_State_GUI ::
	call (const Reference <Messages :: Message <Items :: Character> > & message)
{
	assert (is_initialized ());
	Engines :: Log :: trace (me, Play_State_GUI :: get_class_name (), "call", message . get_name ());

	if (message == Messages :: Message <Items :: Character> :: terminate)
	{
		//	If there are old options, remove them.
		option_map . clear ();
		chat_window -> resetList ();
	}
	else
	{
		assert (message . is_initialized ());
		assert (message . points_to_object ());
		assert (message -> is_initialized ());

		CEGUI :: ListboxTextItem * item = new CEGUI :: ListboxTextItem (message . get_name ());
		option_map [item] = message;

		//	As the chat window is sorted, the item is added in the right place.
		chat_window -> addItem (item);

		chat_window -> ensureItemIsVisible (item);

		Engines :: Log :: log (me) << "Option " << message << " added." << endl;
	}
}

bool Play_State_GUI ::
	handle_event (const CEGUI :: EventArgs & arguments)
{
	Engines :: Log :: trace (me, Play_State_GUI :: get_class_name (), "handle_event", "~arguments~");
	assert (is_initialized ());

	// I had to declare this outside due to scoping.
	boost :: scoped_ptr <const CEGUI :: WindowEventArgs> window_event_arguments;

	#ifdef RADAKAN_TARIQWALJI
		try
		{
	#endif

		window_event_arguments . reset
			(dynamic_cast <const CEGUI :: WindowEventArgs *> (& arguments));
		assert (window_event_arguments);

	#ifdef RADAKAN_TARIQWALJI
		}
		catch (__non_rtti_object e)
		{
			Engines :: Log :: error (me) << "The window's arguments list could not be type casted. Fallig back to unsafe type casts." << endl;

			window_event_arguments . reset ((const CEGUI :: WindowEventArgs *) (& arguments));
		}
	#endif

	if (window_event_arguments -> window -> getType () . find ("Listbox") != string :: npos)
	{
		if (chat_window -> getSelectedCount () != 0)
		{
			Reference <Messages :: Message <Items :: Character> > message
				= option_map [chat_window -> getFirstSelectedItem ()];
			assert (message . points_to_object ());
			assert (message -> is_initialized ());

			chat_window -> clearAllSelections ();

			Observable <Messages :: Message <Items :: Character> > :: call_observers (message);
		}

		return true;
	}
	else
	{
		assert (chat_window -> getFirstSelectedItem () == NULL);
		
		return GUI :: handle_event (arguments);
	}
}
