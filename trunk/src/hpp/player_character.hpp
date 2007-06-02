#ifndef TSL_PLAYER_CHARACTER_HPP
#define TSL_PLAYER_CHARACTER_HPP

#include "character.hpp"
#include "singleton.hpp"

using namespace std;

namespace TSL
{
	namespace Items
	{
		///	Player_Character is the only character that is under direct control of the player.
		class Player_Character :
			public Singleton <Player_Character>,
			public Character
		{
			public :
				Player_Character
				(
					string new_name,
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass
				);
				virtual ~Player_Character ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual bool is_dead () const;
				virtual void die ();
				
				virtual void call (const Object & message);
			
			private :
				bool dead;
		};
	}
}

#endif	//	TSL_PLAYER_CHARACTER_HPP
