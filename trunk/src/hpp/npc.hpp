#ifndef TSL_NPC_HPP
#define TSL_NPC_HPP

#include "character.hpp"
#include "algorithm_state_machine.hpp"

using namespace std;

namespace tsl
{
	class NPC :
		public Character,
		public Algorithm_State_Machine <NPC>
	{
		public :
			virtual ~NPC ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			virtual bool is_dead () const;
			virtual string die ();

			void ai ();
			
			static Item & create
			(
				string new_name,
				string mesh_name,
				float new_volume,
				float new_weight
			);

		protected :
			NPC
			(
				string new_name,
				string mesh_name,
				float new_volume,
				float new_weight
			);

		private :
			NPC (const NPC & npc);
	};
}

#endif	//	TSL_NPC_HPP
