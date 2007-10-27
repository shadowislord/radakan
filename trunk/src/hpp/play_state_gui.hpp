#ifndef RADAKAN_PLAY_STATE_GUI_HPP
#define RADAKAN_PLAY_STATE_GUI_HPP

#include "gui.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		class Character;
	}
	
	namespace Messages
	{
		template <class T> class Message;
	}
	
	class Play_State_GUI :
		public GUI,
		public Observable <Messages :: Message <Items :: Character> >,
		public Observer <Messages :: Message <Items :: Character> >
	{
		public :
			Play_State_GUI
			(
				string new_name,
				boost :: shared_ptr <CEGUI :: Window> new_root
			);
			virtual ~Play_State_GUI ();
			virtual bool is_initialized () const;
			
			static string get_class_name ();

			///	Add 'message' to the conversation options.
			///	Pass 'terminate' to clear the list.
			virtual void call
				(const Reference <Messages :: Message <Items :: Character> > & message);

		protected :
			virtual bool handle_event (const CEGUI :: EventArgs & arguments);
			
		private :
			boost :: shared_ptr <CEGUI :: Listbox> chat_window;
			
			map <CEGUI :: ListboxItem *, Reference <Messages :: Message <Items :: Character> > >
				option_map;
	};
}

#endif	//	RADAKAN_PLAY_STATE_GUI_HPP
