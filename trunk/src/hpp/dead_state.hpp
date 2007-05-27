#ifndef TSL_DEAD_STATE_HPP
#define TSL_DEAD_STATE_HPP

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

		///	Alive_State is the algorithm for dead NPCs.
		class Dead_State :
			public Singleton <Dead_State>,
			public Algorithm <Items :: NPC>
		{
			public :
				Dead_State ();
				virtual ~Dead_State ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual Algorithm <Items :: NPC> & transit (Items :: NPC & owner, const Object & message);

				virtual void enter (Items :: NPC & owner);
		};
	}
}

#endif	//	TSL_DEAD_STATE_HPP
