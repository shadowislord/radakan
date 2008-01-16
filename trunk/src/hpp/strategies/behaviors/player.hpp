#ifndef RADAKAN_STRATEGIES_BEHAVIOR_PLAYER_HPP
#define RADAKAN_STRATEGIES_BEHAVIOR_PLAYER_HPP

#include "strategies/behaviors/behavior.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		namespace Characters
		{
			class Character;
		}
	}
	
	namespace Strategies
	{
		namespace Behaviors
		{
		
			class Player :
				public Behavior
			{
				public :
					static string get_class_name ();
					
					Player (Reference <Items :: Characters :: Character> new_character);
					virtual ~Player ();
					virtual bool is_initialized () const;

					virtual Reference <Behavior> transit
						(const Reference <Messages :: Message <Items :: Characters :: Character> > & message);

			};
		}
	}
}

#endif	//	RADAKAN_STRATEGIES_BEHAVIOR_PLAYER_HPP
