#ifndef TSL_World_HPP
#define TSL_World_HPP

#include "algorithm.hpp"
#include "data_state_machine.hpp"
#include "tile.hpp"
#include "gui.hpp"

using namespace std;

namespace tsl
{
	class TSL;

	class World :
		public Singleton <World>,
		public Algorithm <TSL>,
		public Data_State_Machine <Tile>,
		public OgreOde :: World
	{
		public :
			World (GUI & new_gui, Ogre :: SceneManager & scene_manager, string tsl_path);
			virtual ~World ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			virtual void set_active_state (Tile & tile);
			virtual Algorithm <TSL> & transit (TSL & owner);

		private :
			//	Copies are not allowed.
			World (const World & World);
			
			GUI & gui;
			
			float vertical_camera_angle;

			//	The coordinates of a tile are expressed as (x, z).
			map <Ogre :: Vector3, Tile *> tiles;

			Ogre :: Camera & camera;

			static const int min_x;
			static const int max_x;
			static const int min_z;
			static const int max_z;
	};
}

#endif	//	TSL_World_HPP
