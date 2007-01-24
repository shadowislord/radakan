#ifndef TSL_ALGORITHM_STATE_MACHINE_HPP
#define TSL_ALGORITHM_STATE_MACHINE_HPP

#include "state_machine.hpp"
#include "algorithm.hpp"

using namespace std;

namespace tsl
{
	template <class T> class Algorithm_State_Machine :
		public State_Machine <Algorithm <T> >
	{
		public :
			//	protected constructor, see below
			virtual ~Algorithm_State_Machine ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			///	'set_active_state' requires U to be instantiated already.
			template <typename U> void set_active_state ();
			
			virtual void run ();

		protected :
			Algorithm_State_Machine ();
	};
}

#endif	//	TSL_ALGORITHM_STATE_MACHINE_HPP
