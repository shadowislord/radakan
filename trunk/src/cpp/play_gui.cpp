#include "engines/conversation_engine.hpp"
#include "engines/input_engine.hpp"
#include "engines/log.hpp"
#include "items/characters/character.hpp"
#include "map.hpp"
#include "messages/conversation_message.hpp"
#include "play_gui.hpp"

#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	#include <OgreCEGUIRenderer.h>

	#include <CEGUIWindow.h>
	#include <CEGUIExceptions.h>
	#include <elements/CEGUIListbox.h>
	#include <elements/CEGUIListboxTextItem.h>
	#include <elements/CEGUIPushButton.h>
#endif

using namespace std;
using namespace Radakan;

//	static
string Play_GUI ::
	get_class_name ()
{
	return "Play_GUI";
}

Play_GUI ::
 Play_GUI
	(
		string new_name
		#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
			,
			boost :: shared_ptr <CEGUI :: Window> new_root
		#endif
	) :
	Object (new_name),
	GUI ("Doesn't matter"
	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		, new_root
	#endif
	)
	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		,
		options
			(new Map <CEGUI :: ListboxItem *, Messages :: Message <Items :: Characters :: Character> > ("options"))
	#endif
{
	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
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
	#endif

	Engines :: Conversation_Engine :: get () -> register_observer
		(Reference <Observer <Messages :: Message <Items :: Characters :: Character> > >
			(this));
	Observable <Messages :: Message <Items :: Characters :: Character> >
		:: register_observer (Engines :: Input_Engine :: get ());
	
	assert (is_initialized ());
}

Play_GUI ::
	~Play_GUI ()
{
	Engines :: Log :: trace (me, Play_GUI :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Play_GUI ::
	is_initialized ()
	const
{
	assert (GUI :: is_initialized ());
	
	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		assert (chat_window);
	#endif

	return true;
}

//	virtual
void Play_GUI ::
	call (const Reference <Messages :: Message <Items :: Characters :: Character> > & message)
{
	assert (is_initialized ());

	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		if (message == Messages :: Message <Items :: Characters :: Character> :: terminate)
		{
			//	If there are old options, remove them.
			options -> clear ();
			chat_window -> resetList ();
		}
		else
		{
			assert (message . points_to_object ());
			assert (message -> is_initialized ());

			CEGUI :: ListboxItem * item = new CEGUI :: ListboxTextItem (message . get_name ());
			
			options -> add
			(
				Reference <Pair <CEGUI :: ListboxItem *, Messages :: Message <Items :: Characters :: Character> > >
				(
					new Pair <CEGUI :: ListboxItem *, Messages :: Message <Items :: Characters :: Character> >
						(item, message)
				)
			);

			//	As the chat window is sorted, the item is added in the right place.
			chat_window -> addItem (item);

			chat_window -> ensureItemIsVisible (item);

			Engines :: Log :: log (me) << "Option " << message << " added." << endl;
		}
	#endif
}

#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	bool Play_GUI ::
		handle_event (const CEGUI :: EventArgs & arguments)
	{
		Engines :: Log :: trace (me, Play_GUI :: get_class_name (), "handle_event", "~arguments~");
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
				Reference <Messages :: Message <Items :: Characters :: Character> > message
					= options -> look_up (chat_window -> getFirstSelectedItem ());
				assert (message . points_to_object ());
				assert (message -> is_initialized ());

				chat_window -> clearAllSelections ();

				Observable <Messages :: Message <Items :: Characters :: Character> > :: call_observers (message);
			}

			return true;
		}
		else
		{
			assert (chat_window -> getFirstSelectedItem () == NULL);
			
			return GUI :: handle_event (arguments);
		}
	}
#endif
