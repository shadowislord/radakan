#ifndef RADAKAN_STRATEGIES_BEHAVIORS_BEHAVIOR_HPP
#define RADAKAN_STRATEGIES_BEHAVIORS_BEHAVIOR_HPP

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
			class Behavior :
				public Strategy <Behavior, Items :: Character>
			{
				public :
					static string get_class_name ();
					
					Behavior (Reference <Items :: Character> new_character);
					virtual ~Behavior ();
					virtual bool is_initialized () const;
					
					virtual Reference <Behavior> transit
						(const Reference <Messages :: Message <Items :: Character> > & message) = 0;

					Reference <Items :: Character> character;
			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_BEHAVIORS_BEHAVIOR_HPP
