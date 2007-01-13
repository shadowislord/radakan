#ifndef DEAD_STATE_HPP
#define DEAD_STATE_HPP

#include "state_machine.hpp"
#include "character.hpp"

using namespace std;

namespace tsl
{
	class Dead_State :
		public State <Character>
	{
		public :
			Dead_State (Character & new_owner);
			virtual ~Dead_State ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			virtual string think ();
	};
}

#endif	//	DEAD_STATE_HPP
