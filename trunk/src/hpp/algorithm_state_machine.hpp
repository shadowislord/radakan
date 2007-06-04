#ifndef TSL_ALGORITHM_STATE_MACHINE_HPP
#define TSL_ALGORITHM_STATE_MACHINE_HPP

#include "state_machine.hpp"
#include "algorithm.hpp"

using namespace std;

namespace TSL
{
	namespace Algorithms
	{

		///	I'm the abstract base class for all algorithm based state machines.
		///	Derive from me using private inheritance.
		class Algorithm_State_Machine :
			public State_Machine <Algorithm>
		{
			public :
				Algorithm_State_Machine ();
				virtual ~Algorithm_State_Machine ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual void drop (Object & t, bool stay = false);

				void run (const Object & message, bool old_state_stay = false);
		};
	}
}

#endif	//	TSL_ALGORITHM_STATE_MACHINE_HPP
