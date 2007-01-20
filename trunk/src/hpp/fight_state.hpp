#ifndef FIGHT_STATE_HPP
#define FIGHT_STATE_HPP

#include "state.hpp"

using namespace std;

namespace tsl
{
	class NPC;

	class Fight_State :
		public State <NPC>
	{
		public :
			Fight_State (NPC & new_owner);
			virtual ~Fight_State ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			virtual string run ();
	
		private :
			//	Copies are not allowed.
			Fight_State (const Fight_State & fight_state);
	};
}

#endif	//	FIGHT_STATE_HPP
