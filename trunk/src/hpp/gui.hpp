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
	class ListboxItem;
	class SubscriberSlot;
	class Window;
}

namespace Radakan
{
	namespace Engines
	{
		class Conversation_Engine;
		class Log;
	}

	namespace Messages
	{
		template <class T> class Message;
	}

	///	I'm a graphical user interface.
	class GUI :
		public Resident <GUI>,
		public Observable <Object>,	//	I pass messages to the input engine.
		public Observer <Object>	//	The log pass messages to me.
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

			virtual void call (const Reference <Object> & message);

		protected :
			virtual bool handle_event (const CEGUI :: EventArgs & arguments);

		private :
			void subscribe (CEGUI :: Window & window);

			boost :: shared_ptr <CEGUI :: SubscriberSlot> subscriber;

			boost :: shared_ptr <CEGUI :: Listbox> log_window;
	};
}

#endif	//	RADAKAN_GUI_HPP
