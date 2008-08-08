#ifndef RADAKAN_STATE_MACHINE_HPP
#define RADAKAN_STATE_MACHINE_HPP

#include "slot.hpp"

using namespace std;

namespace Radakan
{

	///	State_Machine is the abstract base class for all state machines.
	template <class T> class State_Machine :
		public Slot <T>
	{
		public :
			///	My constructor is protected, see below.
			virtual ~State_Machine ();
			virtual bool is_initialized () const;

			static string get_class_name ();

			///	'dropped' should not be empty.
			virtual void drop (Reference <T> dropped);

			bool has_active_state () const;
			
			///	'get_active_state ()' requires an active state.
			///	'get_active_state ()' will not be empty.
			Reference <T> get_active_state () const;

			///	'new_state' should not be empty.
			void set_active_state (Reference <T> new_state);

			///	The indices are in chronological order. (0 means first.)
			const boost :: shared_ptr <vector <string> > get_history ();

		protected :
			State_Machine ();

		private :
			///	The history stores the name of previous 'active_state's.
			boost :: shared_ptr <vector <string> > history;
	};
}

#endif	//	RADAKAN_STATE_MACHINE_HPP
