#ifndef RADAKAN_PLAYER_CHARACTER_HPP
#define RADAKAN_PLAYER_CHARACTER_HPP

#include "character.hpp"
#include "singleton.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		///	Player_Character is the only character that is under direct control of the player.
		class Player_Character :
			public Singleton <Player_Character>,
			public Character
		{
			public :
				static string get_class_name ();

				Player_Character
				(
					string new_name,
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass
				);
				virtual ~Player_Character ();
				virtual bool is_initialized () const;
				
				virtual bool is_dead () const;
				virtual void die ();
				
				virtual void call (const Reference <Object> message = update);
			
			private :
				bool dead;
		};
	}
}

#endif	//	RADAKAN_PLAYER_CHARACTER_HPP
