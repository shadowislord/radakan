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

			static string get_class_name ();

			virtual void drop (Reference <T> dropped);

			bool has_active_state () const;
			Reference <T> get_active_state () const;

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
