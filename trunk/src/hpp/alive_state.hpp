#ifndef RADAKAN_ALIVE_STATE_HPP
#define RADAKAN_ALIVE_STATE_HPP

#include "strategy.hpp"
#include "strategy_state_machine.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		class NPC;
	}
	
	namespace Messages
	{
		class Conversation_Message;
	}
	
	namespace Strategies
	{
		///	Alive_State is the basic strategy for living NPCs.
		class Alive_State :
			public Strategy,
			private Strategy_State_Machine
		{
			public :
				static string get_class_name ();
				
				Alive_State (Reference <Items :: NPC> new_npc);
				virtual ~Alive_State ();
				virtual bool is_initialized () const;
				
				virtual Reference <Strategy> transit (Reference <const Object> message);

				Reference <Items :: NPC> npc;
				
				//	'calm' can vary from 0 to 1.
				float calm;

				Reference <Set <const Object> > sensory_buffer;
		};
	}
}

#endif	//	RADAKAN_ALIVE_STATE_HPP
