#ifndef PAUSE_STATE_HPP
#define PAUSE_STATE_HPP

#include "tsl.hpp"

using namespace std;

namespace tsl
{
	class Pause_State :
		public State <TSL>
	{
		public :
			Pause_State (TSL & new_owner);
			virtual ~Pause_State ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			virtual string run ();
	
		private :
			//	Copies are not allowed.
			Pause_State (const Pause_State & pause_state);
	};
}

#endif	//	PAUSE_STATE_HPP
