#ifndef RADAKAN_STRATEGIES_ACTIONS_FIGHT_HPP
#define RADAKAN_STRATEGIES_ACTIONS_FIGHT_HPP

#include "strategies/actions/action.hpp"

using namespace std;

namespace Radakan
{
	namespace Strategies
	{
		namespace Actions
		{
		
			///	Fight is the action strategy for fighting NPCs.
			class Fight :
				public Action
			{
				public :
					static string get_class_name ();
					
					Fight
					(
						Reference <Items :: Characters :: Character> new_character,
						Reference <Behaviors :: AI> new_ai
					);
					virtual ~Fight ();
					virtual bool is_initialized () const;
					
					virtual Reference <Action> transit
						(const Reference <Messages :: Message <Items :: Characters :: Character> > & message);

					virtual string get_action_name ();

				private :
					Reference <Set <Items :: Characters :: Character> > targets;
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_ACTIONS_FIGHT_HPP
