#ifndef TSL_ALIVE_STATE_HPP
#define TSL_ALIVE_STATE_HPP

#include "algorithm_state_machine.hpp"

using namespace std;

namespace TSL
{
	namespace Items
	{
		class NPC;
	}
	
	namespace Algorithms
	{

		///	Alive_State is the basic algorithm for living NPCs.
		class Alive_State :
			public Singleton <Alive_State>,
			public Algorithm <Items :: NPC>
		{
			public :
				Alive_State ();
				virtual ~Alive_State ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				virtual Algorithm <Items :: NPC> & transit (Items :: NPC & owner, const Object & message);
		};
	}
}

#endif	//	TSL_ALIVE_STATE_HPP
