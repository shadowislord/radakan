#ifndef TSL_FIGHT_STATE_HPP
#define TSL_FIGHT_STATE_HPP

#include "algorithm.hpp"

using namespace std;

namespace TSL
{
	namespace Items
	{
		class Character;
	}
	
	namespace Algorithms
	{
		class Alive_State;

		///	Fight_State is the algorithm for fighting NPCs.
		class Fight_State :
			public Algorithm
		{
			public :
				Fight_State (Alive_State & new_alive_state);
				virtual ~Fight_State ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				virtual void transit (const Object & message);

				Alive_State & alive_state;
				Set <Items :: Character> targets;
		};
	}
}

#endif	//	TSL_FIGHT_STATE_HPP
