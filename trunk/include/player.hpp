#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "character.hpp"

using namespace std;

namespace sl
{
	class Player :
		public Character,
		public Ogre :: Singleton <Player>
	{
		public :
			Player (Ogre :: SceneNode * new_node);
			virtual ~Player ();
			virtual bool is_initialized () const;
	};
}

#endif	//	PLAYER_HPP
