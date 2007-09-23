#ifndef RADAKAN_CONVERSATION_ENGINE_HPP
#define RADAKAN_CONVERSATION_ENGINE_HPP

#include "singleton.hpp"

using namespace std;

class TiXmlAttribute;
class TiXmlDocument;
class TiXmlElement;

namespace Radakan
{
	template <class T> class Set;

	namespace Items
	{
		class Character;
	}

	namespace Messages
	{
		class Conversation_Message;
	}

	namespace Engines
	{

		///	comments missing
		class Conversation_Engine :
			public Singleton <Conversation_Engine>
		{
			public :
				Conversation_Engine ();
				virtual ~Conversation_Engine ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				Reference <Set <Messages :: Conversation_Message> > get_options (Reference <Items :: Character > speaker, Reference <Items :: Character> listener);

			private :
				bool evaluate_condition (boost :: shared_ptr <const TiXmlElement> element, Reference <Items :: Character> subject);
				bool evaluate_expression (boost :: shared_ptr <const TiXmlAttribute> attribute, Reference <Items :: Character> subject);
			
				boost :: scoped_ptr <TiXmlDocument> behavior;
		};
	}
}

#endif	// RADAKAN_CONVERSATION_ENGINE_HPP
