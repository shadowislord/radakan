#ifndef RADAKAN_BATTLE_MESSAGE_HPP
#define RADAKAN_BATTLE_MESSAGE_HPP

#include "message.hpp"

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
			public Message <Items :: Character>
		{
			public :
				static string get_class_name ();

				Battle_Message (const string & information, Reference <Items :: Character> new_from, Reference <Items :: Character> new_to);
				virtual ~Battle_Message ();
				virtual bool is_initialized () const;
		};
	}
}

#endif	// RADAKAN_BATTLE_MESSAGE_HPP
