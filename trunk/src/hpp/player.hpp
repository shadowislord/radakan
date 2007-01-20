#ifndef PLAYER_HPP
#define PLAYER_HPP

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
			Player
			(
				float new_volume,
				float new_weight,
				btVector3 new_position,
				Ogre :: SceneNode & new_node
			);
			virtual ~Player ();
			virtual bool is_initialized () const;
			static string get_class_name ();
	};
}

#endif	//	PLAYER_HPP
