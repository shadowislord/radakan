#ifndef AI_MACHINE_HPP
#define AI_MACHINE_HPP

#include "state.hpp"
#include "set.hpp"

using namespace std;

namespace tsl
{
	class State_Machine :
		public State,
		public Set <State>
	{
		public :
			State_Machine ();
			virtual ~State_Machine ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			
			template <typename T> void change_active_state ();
			virtual bool add (State & state);
			template <typename T> void add ();
			virtual string think ();

		private :
			State * active_child_state;
	};
}

#include "state_machine.ipp"

#endif	//	AI_MACHINE_HPP
