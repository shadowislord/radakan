#ifndef TSL_ALGORITHM_STATE_MACHINE_HPP
#define TSL_ALGORITHM_STATE_MACHINE_HPP

#include "state_machine.hpp"
#include "algorithm.hpp"

using namespace std;

namespace TSL
{
	namespace Algorithms
	{

		///	Algorithm_State_Machine is the abstract base class for all algorithm based state machines.
		class Algorithm_State_Machine :
			public State_Machine <Algorithm>,
			protected Algorithm
		{
			public :
				Algorithm_State_Machine ();
				virtual ~Algorithm_State_Machine ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual void drop (Object & t, bool stay);

				virtual void transit (const Object & message);
		};
	}
}

#endif	//	TSL_ALGORITHM_STATE_MACHINE_HPP
