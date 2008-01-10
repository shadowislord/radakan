#ifndef RADAKAN_NPC_HPP
#define RADAKAN_NPC_HPP

#include "items/character.hpp"
#include "strategies/strategy_state_machine.hpp"

using namespace std;

namespace Radakan
{
	class Opinion;

	namespace Items
	{

		///	An NPC is an in-game character that is not under direct control of the player.
		class NPC :
			public Character,
			public Strategies :: Strategy_State_Machine <Items :: Character>
		{
			public :
				static string get_class_name ();

				NPC
				(
					string new_name,
					float new_mass,
					Ogre :: Vector3 new_size,
					const Reference <Mesh_Data> new_mesh_data
				);
				virtual ~NPC ();
				virtual bool is_initialized () const;
				
				virtual void call
					(const Reference <Messages :: Message <Items :: Character> > & message);
				
				virtual bool is_dead () const;
				virtual void die ();

				static Item & create
				(
					string new_name,
					float new_mass,
					Ogre :: Vector3 new_size,
					const Reference <Mesh_Data> new_mesh_data
				);
		};
	}
}

#endif	//	RADAKAN_NPC_HPP
