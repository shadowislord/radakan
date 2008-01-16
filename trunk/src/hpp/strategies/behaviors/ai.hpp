#ifndef RADAKAN_STRATEGIES_BEHAVIOR_AI_HPP
#define RADAKAN_STRATEGIES_BEHAVIOR_AI_HPP

#include "mathematics/bounded_float.hpp"
#include "strategies/behaviors/behavior.hpp"
#include "strategy_state_machine.hpp"

using namespace std;

namespace Radakan
{
	class Opinion;

	namespace Items
	{
		class Character;
	}
	
	namespace Strategies
	{
		namespace Actions
		{
			class Action;
		}
		
		namespace Behaviors
		{
		
			///	AI is the basic strategy for living NPCs.
			class AI :
				public Behavior,
				public Strategy_State_Machine <Actions :: Action, Items :: Character>
			{
				public :
					static string get_class_name ();
					
					AI (Reference <Items :: Character> new_character);
					virtual ~AI ();
					virtual bool is_initialized () const;
					
					virtual Reference <Behavior> transit
						(const Reference <Messages :: Message <Items :: Character> > & message);

					string get_current_action_name () const;

					void set_action (string action_name);

					//	'calm' can vary from 0 to 1.
					Mathematics :: Bounded_Float calm;

				private :
					Reference <Set <Messages :: Message <Items :: Character> > > sensory_buffer;
					Reference <Set <Opinion> > opinions;
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_BEHAVIOR_AI_HPP
