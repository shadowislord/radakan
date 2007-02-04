#ifndef TSL_SECTOR_HPP
#define TSL_SECTOR_HPP

#include <Ogre.h>
#if OGRE_VERSION_MINOR < 3
	#include <OgreKeyEvent.h>
	#include <OgreEventListeners.h>
#endif
#include <OgreStringConverter.h>
#include <OgreException.h>

#include <BulletCollision/BroadphaseCollision/btSimpleBroadphase.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include "battle_engine.hpp"
#include "gui_engine.hpp"
#include "npc.hpp"
#include "player.hpp"

using namespace std;

namespace tsl
{
	class TSL;

	///	Sector keeps track of all data in an in-game sector.

	class Sector :
		public Disjoint_Set <Item>,
		public btDiscreteDynamicsWorld
	{
		public :
			virtual ~Sector ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			///	Don't use 'add' directely, use 'represent' instead.
			virtual bool add (Item & item);
			virtual bool move (Item & item, Disjoint_Set <Item> & destination);

			static Sector & create
			(
				string new_name,
				Ogre :: SceneManager & new_scene_manager
			);

			Ogre :: Camera & get_camera () const;
			Ogre :: SceneManager & get_scene_manager () const;

			//	This can't be a Disjoint_Set,
			//	as the NPCs would be part of diffrent Disjoint_Sets.
			set <NPC *> npcs;
			
		protected :
			Sector
			(
				string new_name,
				Ogre :: SceneManager & new_scene_manager
			);

		private :
			Item & represent
					(Item & item, float x, float y, float z, float scale = 1);
			Ogre :: SceneManager & scene_manager;

		public :
			///	'camera' depends on 'scene_manager' at initialization,
			///	this requires 'camera' to be below 'scene_manager' in this file.
			Ogre :: Camera & camera;
	};
}

#endif	//	TSL_SECTOR_HPP
