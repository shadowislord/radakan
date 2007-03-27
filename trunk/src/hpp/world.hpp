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
		private Data_State_Machine <Tile>,
		public OgreOde :: ExactVariableStepHandler,
		public OgreOde :: CollisionListener
	{
		public :
			World (GUI & new_gui, string tsl_path);
			virtual ~World ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			virtual void set_active_state (Tile & tile);
			virtual Algorithm <TSL> & transit (TSL & owner);

			///	Called by OgreOde whenever a collision occurs,
			///	so that we can modify the contact parameters.
			virtual bool collision (OgreOde :: Contact * contact);

		private :
			//	Copies are not allowed.
			World (const World & World);

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
	};
}

#endif	//	TSL_World_HPP
