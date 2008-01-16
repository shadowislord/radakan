#ifndef RADAKAN_PLAY_GUI_HPP
#define RADAKAN_PLAY_GUI_HPP

#include "gui.hpp"
#include "singleton.hpp"

using namespace std;

#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	namespace CEGUI
	{
		class ListboxItem;
	}
#endif

namespace Radakan
{
	template <typename T, class U> class Map;
	
	namespace Items
	{
		namespace Characters
		{
			class Character;
		}
	}
	
	namespace Messages
	{
		template <class T> class Message;
	}
	
	class Play_GUI :
		protected Singleton <Play_GUI>,
		public GUI,
		public Observable <Messages :: Message <Items :: Characters :: Character> >,
		public Observer <Messages :: Message <Items :: Characters :: Character> >
	{
		public :
			Play_GUI
			(
				string new_name
				#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
					,
					boost :: shared_ptr <CEGUI :: Window> new_root
				#endif
			);
			virtual ~Play_GUI ();
			virtual bool is_initialized () const;
			
			static string get_class_name ();

			///	Add 'message' to the conversation options.
			///	Pass 'terminate' to clear the list.
			virtual void call
				(const Reference <Messages :: Message <Items :: Characters :: Character> > & message);

		protected :
			#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
				virtual bool handle_event (const CEGUI :: EventArgs & arguments);
			#endif

		private :
			#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
				boost :: shared_ptr <CEGUI :: Listbox> chat_window;
				
				Reference <Map <CEGUI :: ListboxItem *, Messages :: Message <Items :: Characters :: Character> > >
					options;
			#endif
	};
}

#endif	//	RADAKAN_PLAY_GUI_HPP
