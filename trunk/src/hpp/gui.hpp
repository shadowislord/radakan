#ifndef RADAKAN_GUI_HPP
#define RADAKAN_GUI_HPP

#include "observable.hpp"
#include "observer.hpp"
#include "resident.hpp"

using namespace std;

namespace CEGUI
{
	class EventArgs;
	class Listbox;
	class SubscriberSlot;
	class Window;
}

namespace Radakan
{
	namespace Engines
	{
		class Log;
	}

	namespace Messages
	{
		class Conversation_Message;
	}

	///	I'm a graphical user interface.
	class GUI :
		public Resident <GUI>,
		public Observable <GUI>,
		public Observer <Engines :: Log>
	{
		public :
			GUI
			(
				string new_name,
				boost :: shared_ptr <CEGUI :: Window> new_root
			);
			virtual ~GUI ();
			virtual bool is_initialized () const;
			
			static string get_class_name ();
			
			boost :: shared_ptr <CEGUI :: Window> root_window;

			virtual void call (const Reference <Object> message = update);

		private :
			void subscribe (CEGUI :: Window & window);

			virtual bool handle_event (const CEGUI :: EventArgs & arguments);

			boost :: shared_ptr <CEGUI :: SubscriberSlot> subscriber;

			boost :: shared_ptr <CEGUI :: Listbox> chat_window;
			
			boost :: shared_ptr <CEGUI :: Listbox> log_window;

			Reference <Set <Messages :: Conversation_Message> > messages;
	};
}

#endif	//	RADAKAN_GUI_HPP
