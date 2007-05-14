#ifndef TSL_ALGORITHM_STATE_MACHINE_HPP
#define TSL_ALGORITHM_STATE_MACHINE_HPP

#include "state_machine.hpp"
#include "algorithm.hpp"
#include <stack>

using namespace std;

namespace TSL
{
	template <class T> class Algorithm_State_Machine :
		public State_Machine <Algorithm <T> >
	{
		public :
			//	protected constructor, see below
			virtual ~Algorithm_State_Machine ();
			virtual bool is_initialized () const;
			
			static const string class_name;

			virtual void run ();
			
			void recall_previous_state ();

		protected :
			Algorithm_State_Machine ();

			stack <Algorithm <T> *> history;
	};
}

#endif	//	TSL_ALGORITHM_STATE_MACHINE_HPP
