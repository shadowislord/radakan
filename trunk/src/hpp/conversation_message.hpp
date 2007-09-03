#ifndef RADAKAN_CONVERSATION_MESSAGE_HPP
#define RADAKAN_CONVERSATION_MESSAGE_HPP

#include "character.hpp"

#include <tinyxml.h>

using namespace std;

namespace Radakan
{
	namespace Messages
	{

		class Conversation_Message :
			public Object
		{
			public :
				Conversation_Message
				(
					const TiXmlElement & new_option,
					Items :: Character & new_from,
					Items :: Character & new_to
				);
				virtual ~Conversation_Message ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				Conversation_Message & get_reaction () const;

				///	'option' is a reference to an XML element in the behavior file.
				const TiXmlElement & option;
				
				Items :: Character & from;
				Items :: Character & to;

			private :
				static string create_name (const TiXmlElement & option);
		};
	}
}

#endif	// RADAKAN_CONVERSATION_MESSAGE_HPP
