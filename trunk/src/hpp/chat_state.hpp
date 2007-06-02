#ifndef TSL_CHAT_STATE_HPP
#define TSL_CHAT_STATE_HPP

#include "algorithm.hpp"

using namespace std;

namespace TSL
{
	namespace Algorithms
	{
		class Alive_State;

		///	Chat_State is the algorithm for conversating NPCs.
		class Chat_State :
			public Algorithm
		{
			public :
				Chat_State (Alive_State & new_alive_state);
				virtual ~Chat_State ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				virtual void transit (const Object & message);

				static Algorithm & create (Alive_State & new_alive_state);

				Alive_State & alive_state;

				static const unsigned int maximal_timeout;

				unsigned int timeout;
		};
	}
}

#endif	//	TSL_CHAT_STATE_HPP
