#ifndef TSL_GAME_HPP
#define TSL_GAME_HPP

#include "algorithm_state_machine.hpp"
#include "model.hpp"
#include "log.hpp"

using namespace std;

namespace TSL
{
	namespace Engines
	{

		///	Game is the main class. It manages the eninges and the sectors.
		class Game :
			public Singleton <Game>,
			public State_Machines :: Algorithm_State_Machine <Game>
		{
			public :
				Game (string tsl_path, string ogre_media_path);
				virtual ~Game ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				void run ();
				string get_FPS () const;

			private :
				Ogre :: Root * root;
				Ogre :: RenderWindow * window;
		};
	}
}

#endif	// TSL_GAME_HPP
