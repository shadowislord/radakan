#ifndef RADAKAN_STATE_MACHINE_HPP
#define RADAKAN_STATE_MACHINE_HPP

#include "location.hpp"

using namespace std;

namespace Radakan
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

			bool has_active_state () const;
			T & get_active_state () const;

			void set_active_state (T & new_state, bool old_state_stay = false);

			///	The indices are in chronological order. (0 means first.)
			const vector <string> & get_history ();

		protected :
			State_Machine ();

		private :
			///	The history stores the name of previous 'active_state's.
			vector <string> history;
	};
}

#endif	//	RADAKAN_STATE_MACHINE_HPP
