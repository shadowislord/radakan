#ifndef RADAKAN_CONVERSATION_ENGINE_HPP
#define RADAKAN_CONVERSATION_ENGINE_HPP

#include "character.hpp"
#include "conversation_message.hpp"
#include "singleton.hpp"

using namespace std;

namespace Radakan
{
	namespace Engines
	{

		class Conversation_Engine :
			public Singleton <Conversation_Engine>
		{
			public :
				Conversation_Engine ();
				virtual ~Conversation_Engine ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				static Set <Messages :: Conversation_Message> & get_options (Items :: Character & speaker, Items :: Character & listener);
				const TiXmlElement & get_reaction (const TiXmlElement & element, Items :: Character & subject);

			private :
				static bool evaluate_condition (const TiXmlElement * element, Items :: Character & subject);
				static bool evaluate_expression (const TiXmlAttribute * attribute, Items :: Character & subject);
			
				TiXmlDocument behavior;
		};
	}
}

#endif	// RADAKAN_CONVERSATION_ENGINE_HPP
