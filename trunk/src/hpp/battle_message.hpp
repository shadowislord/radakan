#ifndef RADAKAN_BATTLE_MESSAGE_HPP
#define RADAKAN_BATTLE_MESSAGE_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		class Character;
	}

	namespace Messages
	{
		class Battle_Message :
			public Object
		{
			public :
				Battle_Message (const string & information, Reference <Items :: Character> new_from, Reference <Items :: Character> new_to);
				virtual ~Battle_Message ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				Reference <Items :: Character> from;
				Reference <Items :: Character> to;
		};
	}
}

#endif	// RADAKAN_BATTLE_MESSAGE_HPP
