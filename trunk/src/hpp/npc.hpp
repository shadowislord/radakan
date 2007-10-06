#ifndef RADAKAN_NPC_HPP
#define RADAKAN_NPC_HPP

#include "character.hpp"
#include "strategy_state_machine.hpp"

using namespace std;

namespace Radakan
{
	class Thought;

	namespace Items
	{

		///	An NPC is an in-game character that is not under direct control of the player.
		class NPC :
			public Character,
			private Strategies :: Strategy_State_Machine
		{
			public :
				static string get_class_name ();

				NPC
				(
					string new_name,
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass
				);
				virtual ~NPC ();
				virtual bool is_initialized () const;
				
				/*virtual void drop (Reference <Item> dropped);
				virtual void drop (Reference <Strategies :: Strategy> dropped);*/

				virtual void call (const Reference <Object> message);
				
				virtual bool is_dead () const;
				virtual void die ();

				void think (string thought) const;
				
				static Item & create
				(
					string new_name,
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass
				);

				Set <Thought> thoughts;
		};
	}
}

#endif	//	RADAKAN_NPC_HPP
