#ifndef SECTOR_HPP
#define SECTOR_HPP

#include "battle_engine.hpp"
#include "npc.hpp"
#include "peace_state.hpp"
#include "fight_state.hpp"
#include "player.hpp"
#include <Ogre.h>
#include <OgreKeyEvent.h>
#include <OgreEventListeners.h>
#include <OgreStringConverter.h>
#include <OgreException.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

using namespace std;

namespace sl
{

	///	Sector keeps track of all data in an in-game sector.

	class Sector :
		public Set <Entity>/* !!!,
		public btDiscreteDynamicsWorld*/
	{
		public :
			Sector (string new_name, Ogre :: SceneManager * new_scene_manager);
			virtual ~Sector ();
			virtual bool is_initialized () const;
			virtual bool add (Entity * entity);
			void update ();
			Ogre :: SceneManager * get_scene_manager () const;
			Ogre :: Camera * get_camera () const;
			Ogre :: SceneNode * copy_node (Ogre :: SceneNode * example);

		private :
			Ogre :: SceneNode * create_entity_node (string name, string mesh_name);
			Ogre :: SceneManager * scene_manager;
			Ogre :: Camera * camera;
			set <NPC *> npcs;	//	This can't be a Set <NPC>, as the NPCs
			//	would be part of diffrent Set <T>s.
	};
}

#endif
