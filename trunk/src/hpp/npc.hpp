#ifndef TSL_NPC_HPP
#define TSL_NPC_HPP

#include "character.hpp"
#include "algorithm_state_machine.hpp"

using namespace std;

namespace TSL
{
	class NPC :
		public Character,
		public Algorithm_State_Machine <NPC>
	{
		public :
			virtual ~NPC ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();

			virtual bool is_dead () const;
			virtual void die ();

			static Item & create
			(
				string new_name,
				string new_mesh_name,
				Ogre :: Vector3 new_size,
				float new_mass
			);

		protected :
			NPC
			(
				string new_name,
				string new_mesh_name,
				Ogre :: Vector3 new_size,
				float new_mass
			);

		private :
			NPC (const NPC & npc);
	};
}

#endif	//	TSL_NPC_HPP
