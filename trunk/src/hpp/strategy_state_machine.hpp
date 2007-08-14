#ifndef RADAKAN_STRATEGY_STATE_MACHINE_HPP
#define RADAKAN_STRATEGY_STATE_MACHINE_HPP

#include "state_machine.hpp"
#include "strategy.hpp"

using namespace std;

namespace Radakan
{
	namespace Strategies
	{

		///	I'm the abstract base class for all strategy based state machines.
		///	Derive me (using private inheritance) for each class that uses strategies.
		class Strategy_State_Machine :
			public State_Machine <Strategy>
		{
			public :
				Strategy_State_Machine ();
				virtual ~Strategy_State_Machine ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual void drop (Object & t, bool stay = false);

				void run (const Object & message, bool old_state_stay = false);
		};
	}
}

#endif	//	RADAKAN_STRATEGY_STATE_MACHINE_HPP
