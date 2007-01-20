#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include "tsl.hpp"

using namespace std;

namespace tsl
{
	class Menu_State :
		public Singleton <Menu_State>,
		public State <TSL>
	{
		public :
			Menu_State (TSL & new_owner);
			virtual ~Menu_State ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			virtual string run ();
	
		private :
			//	Copies are not allowed.
			Menu_State (const Menu_State & menu_state);

			GUI & gui;
	};
}

#endif	//	MENU_STATE_HPP
