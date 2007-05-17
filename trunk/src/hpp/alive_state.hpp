#ifndef TSL_ALIVE_STATE_HPP
#define TSL_ALIVE_STATE_HPP

#include "algorithm_state_machine.hpp"

using namespace std;

namespace TSL
{
	class NPC;

	class Alive_State :
		public Singleton <Alive_State>,
		public Algorithm <NPC>
	{
		public :
			Alive_State ();
			virtual ~Alive_State ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();
			
			virtual Algorithm <NPC> & transit (NPC & owner);
	};
}

#endif	//	TSL_ALIVE_STATE_HPP
