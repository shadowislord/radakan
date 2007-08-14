#ifndef RADAKAN_NPC_HPP
#define RADAKAN_NPC_HPP

#include "character.hpp"
#include "strategy_state_machine.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{

		///	An NPC is an in-game character that is not under direct control of the player.
		class NPC :
			public Character,
			private Strategies :: Strategy_State_Machine
		{
			public :
				NPC
				(
					string new_name,
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass
				);
				virtual ~NPC ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual void drop (Object & t, bool stay);

				virtual void call (const Object & message);
				
				virtual bool is_dead () const;
				virtual void die ();

				void think (const string tought) const;
				
				static Item & create
				(
					string new_name,
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass
				);
		};
	}
}

#endif	//	RADAKAN_NPC_HPP
