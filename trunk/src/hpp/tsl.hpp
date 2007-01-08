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
//		public State_Machine,
		public Ogre :: Singleton <TSL>,
		public Set <Sector>
	{
		public :
			TSL (string tsl_path, string ogre_path);
			~TSL ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			
			void run ();

		private :
			void switch_to (Sector * new_active_sector);

			Ogre :: Root * root;
			Ogre :: RenderWindow * window;
			Battle_Engine battle_engine;
			GUI_Engine * gui_engine;
			Input_Engine * input_engine;
			Ogre :: Timer * timer;
			Sector * active_sector;
			Audio_Engine * audio_engine;
	};
}

#endif
