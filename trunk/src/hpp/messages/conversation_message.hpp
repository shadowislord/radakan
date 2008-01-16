#ifndef RADAKAN_CONVERSATION_MESSAGE_HPP
#define RADAKAN_CONVERSATION_MESSAGE_HPP

#include "message.hpp"

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
			public Message <Items :: Character>
		{
			public :
				static string get_class_name ();

				Conversation_Message
				(
					Reference <Items :: Character> new_from,
					Reference <Items :: Character> new_to,
					const TiXmlElement * new_option
				);
				virtual ~Conversation_Message ();
				virtual bool is_initialized () const;

				//	Overload 'copy ()' in all subclasses.
				virtual Reference <Message <Items :: Character> > copy () const;
				
				Reference <Conversation_Message> get_reaction () const;

				///	'option' is a reference to an XML element in the behavior file.
				const TiXmlElement * option;

			private :
				static string create_name (const TiXmlElement * option);
		};
	}
}

#endif	// RADAKAN_CONVERSATION_MESSAGE_HPP
