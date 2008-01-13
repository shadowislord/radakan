#ifndef RADAKAN_GAME_HPP
#define RADAKAN_GAME_HPP

#include "singleton.hpp"
#include "strategies/strategy_state_machine.hpp"

using namespace std;

namespace Radakan
{
	namespace Engines
	{

		///	Game is the main class. It manages the eninges and the sectors.
		class Game :
			public virtual Object,
			protected Singleton <Game>,
			private Strategies :: Strategy_State_Machine <Game>
		{
			public :
				static string get_class_name ();

				Game (string path_to_config);
				virtual ~Game ();
				virtual bool is_initialized () const;
				
				void run ();

			private :
				boost :: shared_ptr <Ogre :: Root> root;
				boost :: shared_ptr <Ogre :: RenderWindow> window;
				boost :: shared_ptr <Ogre :: SceneManager> scene_manager;
		};
	}
}

#endif	// RADAKAN_GAME_HPP
