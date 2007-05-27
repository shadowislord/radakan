#ifndef TSL_FIGHT_STATE_HPP
#define TSL_FIGHT_STATE_HPP

#include "algorithm.hpp"

using namespace std;

namespace TSL
{
	namespace Items
	{
		class NPC;
	}
	
	namespace Algorithms
	{

		///	Fight_State is the algorithm for fighting NPCs.
		class Fight_State :
			public Singleton <Fight_State>,
			public Algorithm <Items :: NPC>
		{
			public :
				Fight_State ();
				virtual ~Fight_State ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				virtual Algorithm <Items :: NPC> & transit (Items :: NPC & owner, const Object & message);
		};
	}
}

#endif	//	TSL_FIGHT_STATE_HPP
