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
				public Strategy_State_Machine
					<Actions :: Action, Messages :: Communications :: Communication>
			{
				private :
					static boost :: scoped_ptr <map <unsigned int, TiXmlDocument> >
						all_behavior_files;

				public :
					static string get_class_name ();
					
					AI (Reference <Items :: Character> new_character);
					virtual ~AI ();
					virtual bool is_initialized () const;
					
					///	The message is stored.
					virtual void call
						(Reference <Messages :: Communications :: Communication> message);

					///	My FSM will process the last stored message.
					virtual Reference <Behavior> transit
						(Reference <Messages :: Nothing> message);

					virtual bool evaluate_condition (const TiXmlElement * element);
					virtual bool evaluate_expression (const TiXmlAttribute * attribute);

					string get_current_action_name () const;

					void set_action (string action_name);

					virtual const boost :: shared_ptr <set <TiXmlDocument> >
						get_behavior_files () const;

					//	'calm' can vary from 0 to 1.
					Mathematics :: Bounded_Float calm;

				private :
					boost :: scoped_ptr <set <TiXmlDocument> > behavior_files;
					
					Reference <Container <Messages :: Communications :: Communication> >
						sensory_buffer;
					Reference <Container <Opinion> > opinions;
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_BEHAVIOR_AI_HPP
