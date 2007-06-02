#ifndef TSL_STATE_MACHINE_HPP
#define TSL_STATE_MACHINE_HPP

#include "location.hpp"

using namespace std;

namespace TSL
{

	///	State_Machine is the abstract base class for all state machines.
	template <class T> class State_Machine :
		public virtual Object,
		private Location <T>
	{
		public :
			///	My constructor is protected, see below.
			virtual ~State_Machine ();
			virtual bool is_initialized () const;

			static const string get_class_name ();

			virtual void drop (Object & t, bool stay = false);

			virtual bool has_active_state () const;
			virtual T & get_active_state () const;

			virtual void set_active_state (T & new_state);
			virtual void unset_active_state ();

			///	The indices are in chronological order. (0 means first.)
			const vector <string> & get_history ();

		protected :
			State_Machine ();

		private :
			T * active_state;

			///	The history stores the name of previous 'active_state's.
			vector <string> history;
	};
}

#endif	//	TSL_STATE_MACHINE_HPP
