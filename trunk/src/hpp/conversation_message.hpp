#ifndef TSL_CONVERSATION_MESSAGE_HPP
#define TSL_CONVERSATION_MESSAGE_HPP

#include "character.hpp"

using namespace std;

namespace TSL
{
	namespace Messages
	{

		class Conversation_Message :
			public Object
		{
			public :
				Conversation_Message (const string & information, Items :: Character & new_from, Items :: Character & new_to);
				virtual ~Conversation_Message ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				Items :: Character & from;
				Items :: Character & to;
		};
	}
}

#endif	// TSL_CONVERSATION_MESSAGE_HPP
