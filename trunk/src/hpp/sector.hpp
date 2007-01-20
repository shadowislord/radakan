#ifndef SECTOR_HPP
#define SECTOR_HPP

#include "battle_engine.hpp"
#include "gui_engine.hpp"
#include "npc.hpp"
#include "player.hpp"
#include <Ogre.h>
#include <OgreKeyEvent.h>
#include <OgreEventListeners.h>
#include <OgreStringConverter.h>
#include <OgreException.h>

#include <BulletCollision/BroadphaseCollision/btSimpleBroadphase.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

using namespace std;

namespace tsl
{
	class TSL;

	///	Sector keeps track of all data in an in-game sector.

	class Sector :
		public Set <Entity>,
		public State <TSL>,
		public btDiscreteDynamicsWorld
	{
		public :
			Sector
			(
				string new_name,
				TSL & new_owner,
				GUI & new_gui
			);
			virtual ~Sector ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			virtual bool add (Entity & entity);
			virtual bool move_to (Entity & entity, Set <Entity> & other_set);

			virtual string run ();

			Ogre :: Camera & get_camera () const;
			Ogre :: SceneNode & copy_node (Ogre :: SceneNode & example);
			Ogre :: SceneManager & get_scene_manager () const;

			//	This can't be a Set <NPC>,
			//	as the NPCs would be part of diffrent Set <T>s.
			set <NPC *> npcs;

		private :
			Ogre :: SceneNode & create_entity_node (string name, string mesh_name, float scale);
			Ogre :: SceneManager & scene_manager;
			Ogre :: Camera & camera;
			GUI & gui;
	};
}

#endif
