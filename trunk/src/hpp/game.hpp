#ifndef RADAKAN_GAME_HPP
#define RADAKAN_GAME_HPP

#include "strategy_state_machine.hpp"
#include "singleton.hpp"

using namespace std;

namespace Radakan
{
	namespace Engines
	{

		///	Game is the main class. It manages the eninges and the sectors.
		class Game :
			protected Singleton <Game>,
			private Strategies :: Strategy_State_Machine
		{
			public :
				Game (string radakan_path, string ogre_media_path);
				virtual ~Game ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				void run ();

			private :
				Ogre :: Root * root;
				Ogre :: RenderWindow * window;
		};
	}
}

#endif	// RADAKAN_GAME_HPP
