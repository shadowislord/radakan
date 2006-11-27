#ifndef FIGHT_STATE_HPP
#define FIGHT_STATE_HPP

#include "state_machine.hpp"

using namespace std;

namespace sl
{
	class Fight_State :
		public State
	{
		public :
			Fight_State (Character & new_owner);
			virtual ~Fight_State ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			virtual string think ();
	};
}

#endif	//	FIGHT_STATE_HPP
