#ifndef TSL_BATTLE_MESSAGE_HPP
#define TSL_BATTLE_MESSAGE_HPP

#include "character.hpp"

using namespace std;

namespace TSL
{
	namespace Messages
	{

		class Battle_Message :
			public Object
		{
			public :
				Battle_Message (const string & information, Items :: Character & new_from, Items :: Character & new_to);
				virtual ~Battle_Message ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				Items :: Character & from;
				Items :: Character & to;
		};
	}
}

#endif	// TSL_BATTLE_MESSAGE_HPP
