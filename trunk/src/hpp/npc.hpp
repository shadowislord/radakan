#ifndef NPC_HPP
#define NPC_HPP

#include "character.hpp"
#include "state_machine.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "peace_state.hpp"

using namespace std;

namespace tsl
{
	class NPC :
		public Character,
		public State_Machine <NPC>
	{
		public :
			NPC
			(
				float new_volume,
				float new_weight,
				btVector3 new_position,
				Ogre :: SceneNode & new_node
			);
			virtual ~NPC ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			virtual string die ();

		private :
			NPC (const NPC & npc);
	};
}

#endif	//	NPC_HPP
