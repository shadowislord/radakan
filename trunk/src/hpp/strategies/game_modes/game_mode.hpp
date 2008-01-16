#ifndef RADAKAN_STRATEGIES_GAME_MODES_GAME_MODE_HPP
#define RADAKAN_STRATEGIES_GAME_MODES_GAME_MODE_HPP

#include "strategies/strategy.hpp"

using namespace std;

namespace Radakan
{
	class GUI;
	
	namespace Strategies
	{
		namespace Game_Modes
		{
			class Game_Mode :
				public Strategy <Game_Mode, Object>
			{
				public :
					static string get_class_name ();
					
					Game_Mode ();
					virtual ~Game_Mode ();
					virtual bool is_initialized () const;
					
					virtual Reference <Game_Mode> transit
						(const Reference <Messages :: Message <Object> > & message) = 0;

				protected :
					Pointer <GUI> gui;
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_GAME_MODES_GAME_MODE_HPP
