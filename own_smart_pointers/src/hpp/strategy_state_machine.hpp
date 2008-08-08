#ifndef RADAKAN_STRATEGY_STATE_MACHINE_HPP
#define RADAKAN_STRATEGY_STATE_MACHINE_HPP

#include "state_machine.hpp"

using namespace std;

namespace Radakan
{

	///	I'm the abstract base class for all strategy based state machines.
	///	Derive me for each class that uses strategies.
	template <class T, class U> class Strategy_State_Machine :
		public State_Machine <T>
	{
		public :
			static string get_class_name ();

			Strategy_State_Machine ();
			virtual ~Strategy_State_Machine ();
			virtual bool is_initialized () const;

			void run (Reference <U> message);
	};
}

#endif	//	RADAKAN_STRATEGY_STATE_MACHINE_HPP
