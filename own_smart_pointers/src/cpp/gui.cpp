#include "engines/log.hpp"
#include "engines/mediator.hpp"
#include "gui.hpp"
#include "messages/button_event.hpp"
#include "messages/list_event.hpp"
#include "messages/list_update.hpp"

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
string GUI ::
	get_class_name ()
{
	return "GUI";
}

GUI ::
	GUI
	(
		string new_name
		#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
			,
			boost :: shared_ptr <CEGUI :: Window> new_root
		#endif
	) :
	Object (new_name)
	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		,
		root_window (new_root),
		subscriber (new CEGUI :: SubscriberSlot (& GUI :: handle_event, this)),
        lists (new map <string, boost :: shared_ptr <CEGUI :: Listbox> >)
	#endif
{
	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		CEGUI :: Window * temp;

		for (unsigned int i = 0; i < root_window -> getChildCount (); i ++)
		{
			temp = root_window -> getChildAtIdx (i);
			if (temp -> getType () . find ("Listbox"))
			{
				string window_name = temp -> getName () . c_str ();

				//	Convert 'abc-def' to 'def' using 'abc.xml'.
				window_name = without (window_name, without (name, ".xml") + "-");

				Engines :: Log :: log (me) << "window_name: " << window_name << endl;

				#ifdef RADAKAN_TARIQWALJI
					try
					{
				#endif

				lists -> insert
				(
					pair <string, boost :: shared_ptr <CEGUI :: Listbox> >
					(
						window_name, boost :: shared_ptr <CEGUI :: Listbox>
									((CEGUI :: Listbox *) (temp))
					)
				);

				#ifdef RADAKAN_TARIQWALJI
					}
					catch (__non_rtti_object e)
					{
						Engines :: Log :: log (me)
							<< "List '" << window_name
							<< "' could not be dynamically cast. Falling back to unsafe casting."
							<< endl;
						lists -> insert
						(
							pair <string, boost :: shared_ptr <CEGUI :: Listbox> >
							(
								window_name, boost :: shared_ptr <CEGUI :: Listbox>
									((CEGUI :: Listbox *) (temp))
							)
						);
					}
				#endif
			}
		}

		subscribe (* root_window . get ());
	#endif

	add_automatic_destruction_prevention ("construction of " + get_class_name ());
	{
		Engines :: Mediator :: get () -> register_observer <Messages :: List_Update>
			(Reference <Observer <Messages :: List_Update> > (this));
	}
	remove_automatic_destruction_prevention ("construction of " + get_class_name ());

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

	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		assert (lists);
	#endif

	return true;
}

//	virtual
void GUI ::
	call (Reference <Messages :: List_Update> message)
{
	Engines :: Log :: trace (me, GUI :: get_class_name (), "call", message . get_name ());
	assert (is_initialized ());
	assert (message . points_to_object ());
	assert (message -> is_initialized ());
	#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
		assert (0 < lists -> count (message -> list_name));

		boost :: shared_ptr <CEGUI :: Listbox> list
			= lists -> find (message -> list_name) -> second;

		if (message -> reset)
		{
			list -> resetList ();
		}

		for (set <string> :: const_iterator i = message -> items -> begin ();
			i != message -> items -> end (); i ++)
		{
			list -> addItem (new CEGUI :: ListboxTextItem (* i));
			list -> ensureItemIsVisible (list -> getItemCount ());
		}
	#endif
}

#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
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
		Engines :: Log :: trace (me, GUI :: get_class_name (), "handle_event", "@arguments@");
		assert (is_initialized ());

		// I had to declare this outside due to scoping.
		boost :: scoped_ptr <const CEGUI :: WindowEventArgs> window_event_arguments;

		#ifdef RADAKAN_TARIQWALJI
			try
			{
		#endif

			window_event_arguments . reset
				(boost :: polymorphic_cast <const CEGUI :: WindowEventArgs *>
					(& arguments));
			assert (window_event_arguments);

		#ifdef RADAKAN_TARIQWALJI
			}
			catch (__non_rtti_object e)
			{
				Engines :: Log :: error (me)
					<< "The window's arguments list could not be type casted."
					<< "Fallig back to unsafe type casts."
					<< endl;

				window_event_arguments . reset ((const CEGUI :: WindowEventArgs *) (& arguments));
			}
		#endif

		if (window_event_arguments -> window -> getType () . find ("Listbox") != string :: npos)
		{
			boost :: scoped_ptr <CEGUI ::Listbox> listbox
				(boost :: polymorphic_cast <CEGUI ::Listbox *>
					(window_event_arguments -> window));

			if (listbox -> getSelectedCount () != 0)
			{
				string caption (window_event_arguments -> window -> getText () . c_str ());

				Reference <Messages :: List_Event> message
					(new Messages :: List_Event (caption));

				listbox -> clearAllSelections ();

				Engines :: Mediator :: get ()
					-> call_observers <Messages :: List_Event> (message);
			}
		}
		else
		{
			string caption (window_event_arguments -> window -> getText () . c_str ());

			Reference <Messages :: Button_Event> message
				(new Messages :: Button_Event (to_lower_case (caption)));

			Engines :: Mediator :: get ()
				-> call_observers <Messages :: Button_Event> (message);
		}

		return true;
	}
#endif
