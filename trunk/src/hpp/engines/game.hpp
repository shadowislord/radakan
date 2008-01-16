#ifndef RADAKAN_GAME_HPP
#define RADAKAN_GAME_HPP

#include "singleton.hpp"
#include "strategy_state_machine.hpp"

using namespace std;

namespace Radakan
{
	namespace Strategies
	{
		namespace Game_Modes
		{
			class Game_Mode;
		}
	}
	
	namespace Engines
	{

		///	Game is the main class. It manages the eninges and the sectors.
		class Game :
			public virtual Object,
			protected Singleton <Game>,
			private Strategy_State_Machine <Strategies :: Game_Modes :: Game_Mode, Object>
		{
			public :
				static string get_class_name ();

				Game (string path_to_config);
				virtual ~Game ();
				virtual bool is_initialized () const;
				
				void run ();
		};
	}
}

#endif	// RADAKAN_GAME_HPP
