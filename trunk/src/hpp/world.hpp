#ifndef TSL_World_HPP
#define TSL_World_HPP

#include "algorithm.hpp"
#include "battle_engine.hpp"
#include "data_state_machine.hpp"
#include "tile.hpp"
#include "gui.hpp"

using namespace std;

namespace TSL
{
	class Game;

	class World :
		public Singleton <World>,
		public Algorithm <Game>,
		private Environment,
		private Data_State_Machine <Tile>,
		private Battle_Engine,
		public OgreOde :: ExactVariableStepHandler,	//	somehow things don't work without
		public OgreOde :: CollisionListener
	{
		public :
			World (Ogre :: SceneManager & scene_manager, string tsl_path);
			virtual ~World ();
			virtual bool is_initialized () const;
			
			static const string class_name;

			using Singleton <World> :: get;
			using Singleton <World> :: is_instantiated;
			using Singleton <World> :: destruct;
			
			virtual void set_active_state (Tile & tile);
			virtual Algorithm <Game> & transit (Game & owner);

			///	Called by OgreOde whenever a collision occurs,
			///	so that we can modify the contact parameters.
			virtual bool collision (OgreOde :: Contact * contact);

		private :
			void enter (Game & owner);
			
			GUI & gui;
			
			float vertical_camera_angle;

			//	The coordinates of a tile are expressed as (x, z).
			map <pair <int, int>, Tile *> tiles;

			Ogre :: Camera & camera;

			//	This are the tile position limits, not the item position limits!
			static const int min_x;
			static const int max_x;
			static const int min_z;
			static const int max_z;

			///	in radians
			static const float min_vertical_camera_angle;
			
			///	in radians
			static const float max_vertical_camera_angle;

			Movable_Body * player_body;
	};
}

#endif	//	TSL_World_HPP
