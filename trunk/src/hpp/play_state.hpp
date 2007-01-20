#ifndef PLAY_STATE_HPP
#define PLAY_STATE_HPP

#include "tsl.hpp"

using namespace std;

namespace tsl
{
	class Play_State :
		public Singleton <Play_State>,
		public State_Machine <TSL>
	{
		public :
			Play_State (TSL & new_owner);
			virtual ~Play_State ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			virtual void change_active_state (State <TSL> & state);
	
		private :
			//	Copies are not allowed.
			Play_State (const Play_State & play_state);
			
			GUI & gui;
	};
}

#endif	//	PLAY_STATE_HPP
