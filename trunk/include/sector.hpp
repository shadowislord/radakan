#ifndef SECTOR_HPP
#define SECTOR_HPP

#include "battle_engine.hpp"
#include "npc.hpp"
#include "peace_state.hpp"
#include "fight_state.hpp"
#include <Ogre.h>
#include <OgreKeyEvent.h>
#include <OgreEventListeners.h>
#include <OgreStringConverter.h>
#include <OgreException.h>

using namespace std;

///	Sector keeps track of all data in an in-game sector.

class Sector:
	public Set <Entity>
{
	public:
		Sector
			(string new_name,
			Ogre :: SceneManager * new_scene_manager,
			Ogre :: RenderWindow * window);
		virtual ~Sector ();
		virtual bool is_initialized () const;
		virtual bool add (Entity * entity);
		void update ();
		Ogre :: SceneManager * get_scene_manager () const;
		Ogre :: Camera * get_camera () const;
		Character * get_player () const;

	private:
		Ogre :: SceneManager * scene_manager;
		Ogre :: Camera * camera;
		Character * player;
		set <NPC *> npcs;

};

#endif
