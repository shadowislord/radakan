#ifndef TSL_TSL_HPP
#define TSL_TSL_HPP

#include "audio_engine.hpp"
#include "battle_engine.hpp"
#include "input_engine.hpp"
#include "algorithm_state_machine.hpp"

using namespace std;

namespace tsl
{

	///	TSL is the main class. It manages the eninges and the sectors.

	class TSL :
		public Singleton <TSL>,
		public Algorithm_State_Machine <TSL>
	{
		public :
			TSL (string tsl_path, string ogre_path);
			~TSL ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			void run ();
			void set_camera (Ogre :: Camera & new_camera);
			string get_FPS () const;
			Ogre :: SceneManager & new_scene_manager () const;

			///	in milliseconds
			unsigned long get_last_turn_lenght () const;

			const string go_on;
			const string quit;

			Ogre :: RenderWindow * window;

		private :
			unsigned long last_turn_lenght;
			Ogre :: Root * root;
			Ogre :: Timer * turn_lenght_timer;
			Battle_Engine battle_engine;
			GUI_Engine * gui_engine;
			Input_Engine * input_engine;
			Audio_Engine * audio_engine;
	};
}

#endif	//	TSL_TSL_HPP
