#ifndef RADAKAN_STRATEGIES_GAME_MODES_MENU_HPP
#define RADAKAN_STRATEGIES_GAME_MODES_MENU_HPP

#include "singleton.hpp"
#include "strategies/game_modes/game_mode.hpp"

using namespace std;

namespace Radakan
{
	namespace Strategies
	{
		namespace Game_Modes
		{
			///	Menu is the strategy for the game when displaying the menu.
			class Menu :
				public Singleton <Menu>,
				public Game_Mode
			{
				public :
					Menu ();
					virtual ~Menu ();
					virtual bool is_initialized () const;
					
					static string get_class_name ();
					
					virtual Reference <Game_Mode> transit
						(const Reference <Messages :: Message <Object> > & message);
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_GAME_MODES_MENU_HPP
