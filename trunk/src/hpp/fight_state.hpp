#ifndef TSL_FIGHT_STATE_HPP
#define TSL_FIGHT_STATE_HPP

#include "algorithm.hpp"

using namespace std;

namespace tsl
{
	class NPC;

	class Fight_State :
		public Singleton <Fight_State>,
		public Algorithm <NPC>
	{
		public :
			Fight_State ();
			virtual ~Fight_State ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			virtual Algorithm <NPC> & transit (NPC & owner);
	};
}

#endif	//	TSL_FIGHT_STATE_HPP
