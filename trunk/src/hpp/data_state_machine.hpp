#ifndef TSL_DATA_STATE_MACHINE_HPP
#define TSL_DATA_STATE_MACHINE_HPP

#include "disjoint_set.hpp"
#include "state_machine.hpp"

using namespace std;

namespace tsl
{
	template <class T> class Data_State_Machine :
		public State_Machine <T>,
		public Disjoint_Set <T>
	{
		public :
			//	protected constructor, see below
			virtual ~Data_State_Machine ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			///	can only set to a child
			virtual void set_active_state (T & t);
			
			virtual bool add (T & t);

			///	You can not remove states from me, 'move' returns 'false'.
			virtual bool move (T & t, Disjoint_Set <T> & destination);

		protected :
			Data_State_Machine ();
	};
}

#endif	//	TSL_DATA_STATE_MACHINE_HPP
