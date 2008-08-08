#ifndef TSLRPG_HPP
#define TSLRPG_HPP 

#include "sector.hpp"

using namespace std;

///	Tslrpg is a implicit singleton class. It creates the eninges and the sectors.

class Tslrpg:
//	public ExampleApplication,
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
		Sl_Frame_Listener * frame_listener;
		Ogre :: InputReader * input_device;
		Ogre :: RenderWindow * window;
		Battle_Engine battle_engine;
		Sector * active_sector;
		set <Sector *> sectors;
		Character * player;
};

#endif
