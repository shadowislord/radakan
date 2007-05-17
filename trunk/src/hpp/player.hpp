#ifndef TSL_PLAYER_HPP
#define TSL_PLAYER_HPP

#include "character.hpp"
#include "singleton.hpp"

using namespace std;

namespace TSL
{
	class Player :
		public Singleton <Player>,
		public Character
	{
		public :
			virtual ~Player ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();

			virtual bool is_dead () const;
			virtual void die ();
			
			static Item & create
			(
				string new_name,
				string mesh_name,
				Ogre :: Vector3 new_size,
				float new_mass
			);

			float camera_distance;

		protected :
			Player
			(
				string new_name,
				string new_mesh_name,
				Ogre :: Vector3 new_size,
				float new_mass
			);

		private :
			bool dead;
	};
}

#endif	//	TSL_PLAYER_HPP
