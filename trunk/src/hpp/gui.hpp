#ifndef RADAKAN_GUI_HPP
#define RADAKAN_GUI_HPP

#include "observer.hpp"

using namespace std;

#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	namespace CEGUI
	{
		class EventArgs;
		class Listbox;
		class SubscriberSlot;
		class Window;
	}
#endif

namespace Radakan
{
	namespace Messages
	{
		class List_Update;
	}

	///	I'm a graphical user interface.
	class GUI :
		//	I display text messages in lists.
		public Observer <Messages :: List_Update>
	{
		public :
			GUI
			(
				string new_name
				#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
					,
					boost :: shared_ptr <CEGUI :: Window> new_root
				#endif
			);
			virtual ~GUI ();
			virtual bool is_initialized () const;
			
			static string get_class_name ();
			
			#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
				boost :: shared_ptr <CEGUI :: Window> root_window;
			#endif

			virtual void call (Reference <Messages :: List_Update> message);

		protected :
			#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
				virtual bool handle_event (const CEGUI :: EventArgs & arguments);
			#endif

		private :
			#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
				void subscribe (CEGUI :: Window & window);

				boost :: shared_ptr <CEGUI :: SubscriberSlot> subscriber;
				boost :: scoped_ptr <map <string, boost :: shared_ptr <CEGUI :: Listbox> > > lists;
			#endif
	};
}

#endif	//	RADAKAN_GUI_HPP
