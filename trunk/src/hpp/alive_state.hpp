#ifndef TSL_ALIVE_STATE_HPP
#define TSL_ALIVE_STATE_HPP

#include "strategy_state_machine.hpp"

using namespace std;

namespace TSL
{
	namespace Items
	{
		class NPC;
	}
	
	namespace Strategies
	{
		///	Alive_State is the basic strategy for living NPCs.
		class Alive_State :
			public Strategy,
			private Strategy_State_Machine
		{
			public :
				Alive_State (Items :: NPC & new_npc);
				virtual ~Alive_State ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				virtual Strategy * transit (const Object & message);

				Items :: NPC & npc;
				
				//	'calm' can vary from 0 to 1.
				float calm;
		};
	}
}

#endif	//	TSL_ALIVE_STATE_HPP
