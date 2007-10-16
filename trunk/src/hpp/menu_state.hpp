#ifndef RADAKAN_MENU_STATE_HPP
#define RADAKAN_MENU_STATE_HPP

#include "strategy.hpp"
#include "singleton.hpp"

using namespace std;

namespace Radakan
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
				
				static string get_class_name ();
				
				virtual Reference <Strategy> transit (const Reference <Object> & message);
		
			private :
				Reference <GUI> gui;
		};
	}
}

#endif	//	RADAKAN_MENU_STATE_HPP
