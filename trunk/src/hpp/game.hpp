#ifndef TSL_GAME_HPP
#define TSL_GAME_HPP

#include "algorithm_state_machine.hpp"
#include "singleton.hpp"

using namespace std;

namespace TSL
{
	namespace Engines
	{

		///	Game is the main class. It manages the eninges and the sectors.
		class Game :
			protected Singleton <Game>,
			private Algorithms :: Algorithm_State_Machine
		{
			public :
				Game (string tsl_path, string ogre_media_path);
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

#endif	// TSL_GAME_HPP
