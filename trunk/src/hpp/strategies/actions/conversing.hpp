#ifndef RADAKAN_STRATEGIES_ACTIONS_CONVERSING_HPP
#define RADAKAN_STRATEGIES_ACTIONS_CONVERSING_HPP

#include "strategies/actions/action.hpp"

using namespace std;

namespace Radakan
{
	namespace Strategies
	{
		namespace Actions
		{
		
			///	Conversing is the action strategy for conversating NPCs.
			class Conversing :
				public Action
			{
				public :
					static string get_class_name ();
					
					Conversing
					(
						Reference <Items :: Character> new_character,
						Reference <Behaviors :: AI> new_ai
					);
					virtual ~Conversing ();
					virtual bool is_initialized () const;
					
					virtual Reference <Action> transit
						(Reference <Messages :: Communications :: Communication> message);

					virtual string get_action_name ();

				private :
					static const unsigned int maximal_timeout;

					unsigned int timeout;
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_ACTIONS_CONVERSING_HPP
