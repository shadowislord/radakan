#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "character.hpp"

using namespace std;

namespace tsl
{
	class Player :
		public Character,
		public Ogre :: Singleton <Player>
	{
		public :
			Player
			(
				float new_volume,
				float new_weight,
				btVector3 new_position,
				Ogre :: SceneNode & new_node
			);
			virtual ~Player ();
			virtual bool is_initialized () const;
			static string get_type_name ();
	};
}

#endif	//	PLAYER_HPP
