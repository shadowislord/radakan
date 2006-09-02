#ifndef SECTOR_HPP
#define SECTOR_HPP

#include "battle_engine.hpp"
#include "sl_frame_listener.hpp"

using namespace std;

class Sector:
	public Object
{
	public:
		Sector
			(string new_name,
			Ogre :: SceneManager * new_scene_manager,
			Ogre :: RenderWindow * window);
		virtual ~Sector ();
		virtual bool is_initialized () const;
		void run ();
		Sl_Frame_Listener * get_frame_listener () const;
		Character * get_player () const;

	private:
		Ogre :: SceneManager * scene_manager;
		Ogre :: Camera * camera;
		Sl_Frame_Listener * frame_listener;
		Character * player;
		Weapon * sword;
		
		set <Entity *> entities;	//	the items that are currently in this sector

};

#endif
