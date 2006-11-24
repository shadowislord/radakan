#ifndef TSLRPG_HPP
#define TSLRPG_HPP

#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "sector.hpp"

using namespace std;

namespace sl
{

	///	Tslrpg is a implicit singleton class. It creates the eninges and the sectors.

	class Tslrpg :
//		public State_Machine,
		public Set <Sector>,
		public Ogre :: Singleton <Tslrpg>
	{
		public :
			Tslrpg (string sl_path, string ogre_path);
			~Tslrpg ();
			virtual bool is_initialized () const;
			void run ();

		private :
			void switch_to (Sector * new_active_sector);

			float aspect_ratio;

			Ogre :: Root * root;
			Ogre :: RenderWindow * window;

			Battle_Engine battle_engine;
			GUI_Engine * gui_engine;
			Input_Engine * input_engine;
			Ogre :: Timer * timer;

			Sector * active_sector;
	};
}

#endif
