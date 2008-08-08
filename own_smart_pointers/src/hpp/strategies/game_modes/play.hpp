#ifndef RADAKAN_STRATEGIES_GAME_MODES_PLAY_HPP
#define RADAKAN_STRATEGIES_GAME_MODES_PLAY_HPP

#include "singleton.hpp"
#include "strategies/game_modes/game_mode.hpp"

using namespace std;

namespace Radakan
{
	namespace Strategies
	{
		namespace Game_Modes
		{
			class Play :
				public Game_Mode,
				public Singleton <Play>
			{
				public :
					static string get_class_name ();

					Play ();
					virtual ~Play ();
					virtual bool is_initialized () const;
					
					virtual Reference <Game_Mode> transit
						(Reference <Messages :: Nothing> message);
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_GAME_MODES_PLAY_HPP
