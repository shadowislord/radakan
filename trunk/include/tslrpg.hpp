#ifndef TSLRPG_HPP
#define TSLRPG_HPP

#include "input_engine.hpp"
#include "sector.hpp"

using namespace std;

///	Tslrpg is a implicit singleton class. It creates the eninges and the sectors.

class Tslrpg:
	public Object
{
	public:
		Tslrpg (string path);
		~Tslrpg ();
		virtual bool is_initialized () const;
		void run ();

	private:
		void create_scene ();

		Ogre :: Root * root;
		Ogre :: RenderWindow * window;

		Input_Engine * input_engine;

		Battle_Engine battle_engine;

		Sector * active_sector;
		set <Sector *> sectors;

		Character * player;
};

#endif
