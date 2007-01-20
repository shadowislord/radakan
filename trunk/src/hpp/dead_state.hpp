#ifndef DEAD_STATE_HPP
#define DEAD_STATE_HPP

#include "state.hpp"

using namespace std;

namespace tsl
{
	class NPC;

	class Dead_State :
		public State <NPC>
	{
		public :
			Dead_State (NPC & new_owner);
			virtual ~Dead_State ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			virtual string run ();
	
		private :
			//	Copies are not allowed.
			Dead_State (const Dead_State & dead_state);
	};
}

#endif	//	DEAD_STATE_HPP
