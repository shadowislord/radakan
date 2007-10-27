#ifndef RADAKAN_STRATEGY_STATE_MACHINE_HPP
#define RADAKAN_STRATEGY_STATE_MACHINE_HPP

#include "state_machine.hpp"

using namespace std;

namespace Radakan
{
	namespace Messages
	{
		template <class T> class Message;
	}

	namespace Strategies
	{
		template <class T> class Strategy;

		///	I'm the abstract base class for all strategy based state machines.
		///	Derive me for each class that uses strategies.
		template <class T> class Strategy_State_Machine :
			public State_Machine <Strategy <T> >
		{
			public :
				static string get_class_name ();

				Strategy_State_Machine ();
				virtual ~Strategy_State_Machine ();
				virtual bool is_initialized () const;
				
				void run (const Reference <Messages :: Message <T> > & message);
		};
	}
}

#endif	//	RADAKAN_STRATEGY_STATE_MACHINE_HPP
