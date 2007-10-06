#ifndef RADAKAN_STRATEGY_STATE_MACHINE_HPP
#define RADAKAN_STRATEGY_STATE_MACHINE_HPP

#include "state_machine.hpp"

using namespace std;

namespace Radakan
{
	namespace Strategies
	{
		class Strategy;

		///	I'm the abstract base class for all strategy based state machines.
		///	Derive me (using private inheritance) for each class that uses strategies.
		class Strategy_State_Machine :
			public State_Machine <Strategy>
		{
			public :
				static string get_class_name ();

				Strategy_State_Machine ();
				virtual ~Strategy_State_Machine ();
				virtual bool is_initialized () const;
				
				void run (const Reference <Object> message);
		};
	}
}

#endif	//	RADAKAN_STRATEGY_STATE_MACHINE_HPP
