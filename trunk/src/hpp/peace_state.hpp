#ifndef PEACE_STATE_HPP
#define PEACE_STATE_HPP

#include "state_machine.hpp"

using namespace std;

namespace tsl
{
	class Peace_State :
		public State
	{
		public :
			Peace_State (Character & new_owner);
			virtual ~Peace_State ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			virtual string think ();
	};
}

#endif	//	PEACE_STATE_HPP
