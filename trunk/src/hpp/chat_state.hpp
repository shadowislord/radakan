#ifndef RADAKAN_CHAT_STATE_HPP
#define RADAKAN_CHAT_STATE_HPP

#include "strategy.hpp"

using namespace std;

namespace Radakan
{
	namespace Strategies
	{
		class Alive_State;

		///	Chat_State is the strategy for conversating NPCs.
		class Chat_State :
			public Strategy
		{
			public :
				static string get_class_name ();
				
				Chat_State (Reference <Alive_State> new_alive_state);
				virtual ~Chat_State ();
				virtual bool is_initialized () const;
				
				virtual Reference <Strategy> transit (Reference <const Object> message);

				Reference <Alive_State> alive_state;

				static const unsigned int maximal_timeout;

				unsigned int timeout;
		};
	}
}

#endif	//	RADAKAN_CHAT_STATE_HPP
