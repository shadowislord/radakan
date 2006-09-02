#ifndef TSLRPG_HPP
#define TSLRPG_HPP 

#include "sector.hpp"

using namespace std;

class Tslrpg:
//	public ExampleApplication,
	public Object
{
	public:
		Tslrpg ();
		~Tslrpg ();
		virtual bool is_initialized () const;
		void run ();

	private:
		void create_scene ();
		Ogre :: Root * root;
		Ogre :: InputReader * input_device;
		Ogre :: RenderWindow * window;
		Battle_Engine battle_engine;
		Sector * active_sector;
		set <Sector *> sectors;
		Character * player;
};

#endif
