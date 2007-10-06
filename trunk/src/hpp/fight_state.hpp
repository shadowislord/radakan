#ifndef RADAKAN_FIGHT_STATE_HPP
#define RADAKAN_FIGHT_STATE_HPP

#include "strategy.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		class Character;
	}
	
	namespace Strategies
	{
		class Alive_State;

		///	Fight_State is the strategy for fighting NPCs.
		class Fight_State :
			public Strategy
		{
			public :
				Fight_State (Reference <Alive_State> new_alive_state);
				virtual ~Fight_State ();
				virtual bool is_initialized () const;
				
				static string get_class_name ();
				
				virtual Reference <Strategy> transit (const Reference <Object> message);

				Reference <Alive_State> alive_state;

			private :
				Reference <Set <Items :: Character> > targets;
		};
	}
}

#endif	//	RADAKAN_FIGHT_STATE_HPP
