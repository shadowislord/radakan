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
			Player (Ogre :: SceneNode & new_node);
			virtual ~Player ();
			virtual bool is_initialized () const;
			static string get_type_name ();
	};
}

#endif	//	PLAYER_HPP
