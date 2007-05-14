#ifndef TSL_DEAD_STATE_HPP
#define TSL_DEAD_STATE_HPP

#include "algorithm.hpp"

using namespace std;

namespace TSL
{
	class NPC;

	class Dead_State :
		public Singleton <Dead_State>,
		public Algorithm <NPC>
	{
		public :
			Dead_State ();
			virtual ~Dead_State ();
			virtual bool is_initialized () const;
			
			static const string class_name;

			virtual void enter (NPC & owner);
	};
}

#endif	//	TSL_DEAD_STATE_HPP
