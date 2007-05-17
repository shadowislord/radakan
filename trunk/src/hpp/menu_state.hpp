#ifndef TSL_MENU_STATE_HPP
#define TSL_MENU_STATE_HPP

#include "game.hpp"
#include "gui.hpp"

using namespace std;

namespace TSL
{
	class Menu_State :
		public Singleton <Menu_State>,
		public Algorithm <Game>
	{
		public :
			Menu_State ();
			virtual ~Menu_State ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();
			
			virtual Algorithm <Game> & transit (Game & owner);
	
		private :
			void enter (Game & owner);
			
			GUI & gui;
	};
}

#endif	//	TSL_MENU_STATE_HPP
