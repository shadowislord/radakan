#ifndef RADAKAN_CHARACTERS_PLAYER_CHARACTER_HPP
#define RADAKAN_CHARACTERS_PLAYER_CHARACTER_HPP

#include "character.hpp"
#include "singleton.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		namespace Characters
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
						float new_mass,
						Ogre :: Vector3 new_size,
						const Reference <Mesh_Data> new_mesh_data
					);
					virtual ~Player_Character ();
					virtual bool is_initialized () const;

					virtual void call
						(const Reference <Messages :: Message <Character> > & message);
			};
		}
	}
}

#endif	//	RADAKAN_CHARACTERS_PLAYER_CHARACTER_HPP
