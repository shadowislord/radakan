#ifndef RADAKAN_ALIVE_STATE_HPP
#define RADAKAN_ALIVE_STATE_HPP

#include "strategy.hpp"
#include "strategy_state_machine.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		class Character;
	}
	
	namespace Messages
	{
		class Conversation_Message;
	}
	
	namespace Strategies
	{
		///	Alive_State is the basic strategy for living NPCs.
		class Alive_State :
			public Strategy <Items :: Character>,
			public Strategy_State_Machine <Items :: Character>
		{
			public :
				static string get_class_name ();
				
				Alive_State (Reference <Items :: Character> new_character);
				virtual ~Alive_State ();
				virtual bool is_initialized () const;
				
				virtual Reference <Strategy <Items :: Character> > transit
					(const Reference <Messages :: Message <Items :: Character> > & message);

				///	'set_active_state_class' assures that my active state class
				///	name is 'class_name'.
				void set_active_state_class (string class_name);

				//	'calm' can vary from 0 to 1.
				float calm;

			private :
				Reference <Items :: Character> character;
							
				Reference <Set <Messages :: Message <Items :: Character> > > sensory_buffer;
		};
	}
}

#endif	//	RADAKAN_ALIVE_STATE_HPP
