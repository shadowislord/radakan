#ifndef RADAKAN_STRATEGIES_ACTIONS_FIGHTING_HPP
#define RADAKAN_STRATEGIES_ACTIONS_FIGHTING_HPP

#include "strategies/actions/action.hpp"

using namespace std;

namespace Radakan
{
	namespace Strategies
	{
		namespace Actions
		{
		
			///	Fighting is the action strategy for fighting NPCs.
			class Fighting :
				public Action
			{
				public :
					static string get_class_name ();
					
					Fighting
					(
						Reference <Items :: Character> new_character,
						Reference <Behaviors :: AI> new_ai
					);
					virtual ~Fighting ();
					virtual bool is_initialized () const;
					
					virtual Reference <Action> transit
						(Reference <Messages :: Communications :: Communication> message);

					virtual string get_action_name ();

				private :
					Reference <Set <Items :: Character> > targets;
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_ACTIONS_FIGHTING_HPP
