#ifndef RADAKAN_STRATEGIES_ACTIONS_ACTION_HPP
#define RADAKAN_STRATEGIES_ACTIONS_ACTION_HPP

#include "strategies/strategy.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		class Character;
	}
	
	namespace Strategies
	{
		namespace Behaviors
		{
			class AI;
		}

		namespace Actions
		{
			class Action :
				public Strategy <Action, Items :: Character>
			{
				public :
					static string get_class_name ();
					
					Action
					(
						Reference <Items :: Character> new_character,
						Reference <Behaviors :: AI> new_ai
					);
					virtual ~Action ();
					virtual bool is_initialized () const;
					
					virtual Reference <Action> transit
						(const Reference <Messages :: Message <Items :: Character> > & message) = 0;
					//	'get_action_name ()' returns the name of a subclass.
					virtual string get_action_name () = 0;

					Reference <Items :: Character> character;
					Reference <Behaviors :: AI> ai;
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_ACTIONS_ACTION_HPP
