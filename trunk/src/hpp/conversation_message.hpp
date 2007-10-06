#ifndef RADAKAN_CONVERSATION_MESSAGE_HPP
#define RADAKAN_CONVERSATION_MESSAGE_HPP

#include "object.hpp"

using namespace std;

class TiXmlElement;

namespace Radakan
{
	namespace Items
	{
		class Character;
	}

	namespace Messages
	{

		//	comments missing
		class Conversation_Message :
			public Object
		{
			public :
				static string get_class_name ();

				Conversation_Message
				(
					const TiXmlElement * new_option,
					Reference <Items :: Character> new_from,
					Reference <Items :: Character> new_to
				);
				virtual ~Conversation_Message ();
				virtual bool is_initialized () const;
				
				Reference <Conversation_Message> get_reaction () const;

				///	'option' is a reference to an XML element in the behavior file.
				const TiXmlElement * option;
				
				Reference <Items :: Character> from;
				Reference <Items :: Character> to;

			private :
				static string create_name (const TiXmlElement * option);
		};
	}
}

#endif	// RADAKAN_CONVERSATION_MESSAGE_HPP
