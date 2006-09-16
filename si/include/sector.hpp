#ifndef SECTOR_HPP
#define SECTOR_HPP

#include "battle_engine.hpp"
#include "sl_frame_listener.hpp"

using namespace std;

///	Sector keeps track of all data in an in-game sector.

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
		Ogre :: SceneManager * get_scene_manager () const;
		Ogre :: Camera * get_camera () const;
		Character * get_player () const;
		Container items;

	private:
		Ogre :: SceneManager * scene_manager;
		Ogre :: Camera * camera;
		Character * player;
};

#endif
