#ifndef PEACE_STATE_HPP
#define PEACE_STATE_HPP

#include "state.hpp"

using namespace std;

namespace tsl
{
	class NPC;

	class Peace_State :
		public State <NPC>
	{
		public :
			Peace_State (NPC & new_owner);
			virtual ~Peace_State ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			virtual string run ();
	
		private :
			//	Copies are not allowed.
			Peace_State (const Peace_State & peace_state);
	};
}

#endif	//	PEACE_STATE_HPP
