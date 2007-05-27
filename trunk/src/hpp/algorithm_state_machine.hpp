#ifndef TSL_ALGORITHM_STATE_MACHINE_HPP
#define TSL_ALGORITHM_STATE_MACHINE_HPP

#include "state_machine.hpp"
#include "algorithm.hpp"
#include <stack>

using namespace std;

namespace TSL
{
	namespace State_Machines
	{

		///	Algorithm_State_Machine is the abstract base class for all algorithm based state machines.
		template <class T> class Algorithm_State_Machine :
			public State_Machine <Algorithms :: Algorithm <T> >
		{
			public :
				//	protected constructor, see below
				virtual ~Algorithm_State_Machine ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual void run (const Object & message);
				virtual void set_active_state (Algorithms :: Algorithm <T> & new_state);
				
				void recall_previous_state ();

			protected :
				Algorithm_State_Machine ();

				stack <Algorithms :: Algorithm <T> *> history;

			private :
				T & get_owner () const;
		};
	}
}

#endif	//	TSL_ALGORITHM_STATE_MACHINE_HPP
