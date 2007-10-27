#ifndef RADAKAN_FIGHT_STATE_HPP
#define RADAKAN_FIGHT_STATE_HPP

#include "strategy.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		class Character;
	}
	
	namespace Strategies
	{
		class Alive_State;

		///	Fight_State is the strategy for fighting NPCs.
		class Fight_State :
			public Strategy <Items :: Character>
		{
			public :
				Fight_State (Reference <Items :: Character> new_character, Reference <Alive_State> new_alive_state);
				virtual ~Fight_State ();
				virtual bool is_initialized () const;
				
				static string get_class_name ();
				
				virtual Reference <Strategy <Items :: Character> > transit
					(const Reference <Messages :: Message <Items :: Character> > & message);

			private :
				Reference <Items :: Character> character;

				Reference <Alive_State> alive_state;

				Reference <Set <Items :: Character> > targets;
		};
	}
}

#endif	//	RADAKAN_FIGHT_STATE_HPP
