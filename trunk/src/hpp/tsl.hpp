#ifndef TSL_HPP
#define TSL_HPP

#include "audio_engine.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "sector.hpp"

using namespace std;

namespace tsl
{

	///	TSL is the main class. It manages the eninges and the sectors.

	class TSL :
		public State_Machine <TSL>,
		public Ogre :: Singleton <TSL>
	{
		friend class Play_State;
		friend class Sector;
	
		public :
			TSL (string tsl_path, string ogre_path);
			~TSL ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			bool quit;

		private :
			//	This returns NULL, if no sector is active.
			Sector * get_active_sector ();
			
			Ogre :: Root * root;
			Ogre :: RenderWindow * window;
			Battle_Engine battle_engine;
			GUI_Engine * gui_engine;
			Input_Engine * input_engine;
			Ogre :: Timer * timer;
			Audio_Engine * audio_engine;
	};
}

#endif
