#ifndef TSL_MENU_STATE_HPP
#define TSL_MENU_STATE_HPP

#include "strategy.hpp"
#include "singleton.hpp"

using namespace std;

namespace TSL
{
	class GUI;

	namespace Engines
	{
		class Game;
	}

	namespace Strategies
	{
		///	Menu_State is the strategy for the game when displaying the menu.
		class Menu_State :
			public Singleton <Menu_State>,
			public Strategy
		{
			public :
				Menu_State ();
				virtual ~Menu_State ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				virtual Strategy * transit (const Object & message);
		
			private :
				GUI & gui;
		};
	}
}

#endif	//	TSL_MENU_STATE_HPP
