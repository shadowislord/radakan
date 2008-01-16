#ifndef RADAKAN_STRATEGIES_ACTIONS_CONVERSATE_HPP
#define RADAKAN_STRATEGIES_ACTIONS_CONVERSATE_HPP

#include "strategies/actions/action.hpp"

using namespace std;

namespace Radakan
{
	namespace Strategies
	{
		namespace Actions
		{
		
			///	Conversate is the action strategy for conversating NPCs.
			class Conversate :
				public Action
			{
				public :
					static string get_class_name ();
					
					Conversate
					(
						Reference <Items :: Character> new_character,
						Reference <Behaviors :: AI> new_ai
					);
					virtual ~Conversate ();
					virtual bool is_initialized () const;
					
					virtual Reference <Action> transit
						(const Reference <Messages :: Message <Items :: Character> > & message);

					virtual string get_action_name ();

				private :
					static const unsigned int maximal_timeout;

					unsigned int timeout;
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_ACTIONS_CONVERSATE_HPP
