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
			public virtual Object,
			protected Singleton <Game>,
			private Strategies :: Strategy_State_Machine
		{
			public :
				static string get_class_name ();

				Game (string radakan_path, string ogre_media_path);
				virtual ~Game ();
				virtual bool is_initialized () const;
				
				void run ();

			private :
				boost :: shared_ptr <Ogre :: Root> root;
				boost :: shared_ptr <Ogre :: RenderWindow> window;
		};
	}
}

#endif	// RADAKAN_GAME_HPP
