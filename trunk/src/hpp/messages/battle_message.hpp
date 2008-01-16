#ifndef RADAKAN_BATTLE_MESSAGE_HPP
#define RADAKAN_BATTLE_MESSAGE_HPP

#include "message.hpp"

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

	namespace Messages
	{
		class Battle_Message :
			public Message <Items :: Characters :: Character>
		{
			public :
				static string get_class_name ();

				Battle_Message (const string & information, Reference <Items :: Characters :: Character> new_from, Reference <Items :: Characters :: Character> new_to);
				virtual ~Battle_Message ();
				virtual bool is_initialized () const;
				
				//	Overload 'copy ()' in all subclasses.
				virtual Reference <Message <Items :: Characters :: Character> > copy () const;
		};
	}
}

#endif	// RADAKAN_BATTLE_MESSAGE_HPP
