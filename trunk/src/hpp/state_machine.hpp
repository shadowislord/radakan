#ifndef TSL_STATE_MACHINE_HPP
#define TSL_STATE_MACHINE_HPP

#include "object.hpp"

using namespace std;

namespace TSL
{

	///	State_Machine is the abstract base class for all state machines.
	template <class T> class State_Machine :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~State_Machine ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();

			virtual bool has_active_state () const;
			virtual T & get_active_state () const;
			virtual void set_active_state (T & new_state);
			virtual void unset_active_state ();

		protected :
			State_Machine ();

		private :
			T * active_state;
	};
}

#endif	//	TSL_STATE_MACHINE_HPP
