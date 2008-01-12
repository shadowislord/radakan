#ifndef RADAKAN_GAME_HPP
#define RADAKAN_GAME_HPP

#include <OgreConfigFile.h>

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

				Game ();
				virtual ~Game ();
				virtual bool is_initialized () const;
				
				void run ();

			private :
				boost :: shared_ptr <Ogre :: Root> root;
				boost :: shared_ptr <Ogre :: RenderWindow> window;
				boost :: shared_ptr <Ogre :: SceneManager> scene_manager;

				// For loading the radakan settings from a file
				Ogre::ConfigFile *radakan_config;


				string radakan_path;
				string ogre_media_path;
		};
	}
}

#endif	// RADAKAN_GAME_HPP
