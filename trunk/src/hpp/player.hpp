#ifndef TSL_PLAYER_HPP
#define TSL_PLAYER_HPP

#include "character.hpp"
#include "singleton.hpp"

using namespace std;

namespace tsl
{
	class Player :
		public Singleton <Player>,
		public Character
	{
		public :
			virtual ~Player ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			virtual bool is_dead () const;
			virtual string die ();
			
			static Item & create
			(
				string new_name,
				string mesh_name,
				float new_volume,
				float new_weight
			);

		protected :
			Player
			(
				string new_name,
				string mesh_name,
				float new_volume,
				float new_weight
			);

		private :
			bool dead;
	};
}

#endif	//	TSL_PLAYER_HPP
