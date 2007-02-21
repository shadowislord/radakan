#ifndef TSL_MENU_STATE_HPP
#define TSL_MENU_STATE_HPP

#include "tsl.hpp"
#include "gui.hpp"

using namespace std;

namespace tsl
{
	class Menu_State :
		public Singleton <Menu_State>,
		public Algorithm <TSL>
	{
		public :
			Menu_State ();
			virtual ~Menu_State ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			virtual Algorithm <TSL> & transit (TSL & owner);
	
		private :
			//	Copies are not allowed.
			Menu_State (const Menu_State & menu_state);

			GUI & gui;
	};
}

#endif	//	TSL_MENU_STATE_HPP
