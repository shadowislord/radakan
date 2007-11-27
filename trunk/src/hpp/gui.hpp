#ifndef RADAKAN_GUI_HPP
#define RADAKAN_GUI_HPP

#include "observable.hpp"
#include "observer.hpp"
#include "resident.hpp"

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

			virtual void call (const Reference <Object> & message);

		protected :
			#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
				virtual bool handle_event (const CEGUI :: EventArgs & arguments);
			#endif

		private :
			#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
				void subscribe (CEGUI :: Window & window);

				boost :: shared_ptr <CEGUI :: SubscriberSlot> subscriber;

				boost :: shared_ptr <CEGUI :: Listbox> log_window;
			#endif

	};
}

#endif	//	RADAKAN_GUI_HPP
