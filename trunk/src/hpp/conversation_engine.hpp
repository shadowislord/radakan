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
				static string get_class_name ();

				Conversation_Engine ();
				virtual ~Conversation_Engine ();
				virtual bool is_initialized () const;
				
				Reference <Set <Messages :: Conversation_Message> > get_options (Reference <Items :: Character > speaker, Reference <Items :: Character> listener);

			private :
				bool evaluate_condition (const TiXmlElement * element, Reference <Items :: Character> subject);
				bool evaluate_expression (const TiXmlAttribute * attribute, Reference <Items :: Character> subject);
			
				boost :: scoped_ptr <TiXmlDocument> behavior;
		};
	}
}

#endif	// RADAKAN_CONVERSATION_ENGINE_HPP
