#ifndef TSL_PEACE_STATE_HPP
#define TSL_PEACE_STATE_HPP

#include "algorithm_state_machine.hpp"

using namespace std;

namespace tsl
{
	class NPC;

	class Peace_State :
		public Singleton <Peace_State>,
		public Algorithm <NPC>
	{
		public :
			Peace_State ();
			virtual ~Peace_State ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			virtual Algorithm <NPC> & transit (NPC & owner);
	
		private :
			//	Copies are not allowed.
			Peace_State (const Peace_State & peace_state);
	};
}

#endif	//	TSL_PEACE_STATE_HPP
