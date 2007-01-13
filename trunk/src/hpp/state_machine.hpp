#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include "state.hpp"

using namespace std;

namespace tsl
{

	///	State_Machine is the abstract base class for all finite state machines.

	template <typename T> class State_Machine :
		public State <T>,
		public Set <State <T> >
	{
		public :
			virtual ~State_Machine ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			
			template <typename U> void change_active_state ();
			virtual bool add (State <T> & state);
			template <typename U> void add ();
			virtual string think ();

		protected:
			State_Machine (T * new_owner);	//	using a reference gives type problems

		private :
			State <T> * active_child_state;
	};
}

#endif	//	STATE_MACHINE_HPP
