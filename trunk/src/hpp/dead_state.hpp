#ifndef TSL_DEAD_STATE_HPP
#define TSL_DEAD_STATE_HPP

#include "algorithm.hpp"

using namespace std;

namespace tsl
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
			static string get_class_name ();
			
			virtual Algorithm <NPC> & transit (NPC & owner);
	
		private :
			//	Copies are not allowed.
			Dead_State (const Dead_State & dead_state);

			Ogre :: Quaternion & orientation;
	};
}

#endif	//	TSL_DEAD_STATE_HPP
