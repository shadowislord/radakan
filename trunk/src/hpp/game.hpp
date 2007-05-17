#ifndef TSL_GAME_HPP
#define TSL_GAME_HPP

#include "algorithm_state_machine.hpp"
#include "model.hpp"
#include "log.hpp"

using namespace std;

namespace TSL
{

	///	Game is the main class. It manages the eninges and the sectors.
	class Game :
		public Singleton <Game>,
		public Algorithm_State_Machine <Game>
	{
		public :
			Game (string tsl_path, string ogre_media_path);
			virtual ~Game ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();

			void run ();
			void set_camera (Ogre :: Camera & new_camera);
			string get_FPS () const;

			///	in seconds
			const float & get_last_turn_lenght () const;

			Ogre :: RenderWindow * window;

		private :
			float last_turn_lenght;
			Ogre :: Root * root;
			Ogre :: Timer turn_lenght_timer;
	};
}

#endif	// TSL_GAME_HPP
