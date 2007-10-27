#ifndef RADAKAN_CONVERSATION_ENGINE_HPP
#define RADAKAN_CONVERSATION_ENGINE_HPP

#include "observable.hpp"
#include "singleton.hpp"

using namespace std;

class TiXmlAttribute;
class TiXmlDocument;
class TiXmlElement;

namespace Radakan
{
	namespace Items
	{
		class Character;
	}

	namespace Messages
	{
		template <class T> class Message;
	}

	namespace Engines
	{

		///	comments missing
		class Conversation_Engine :
			public Singleton <Conversation_Engine>,
			public Observable <Messages :: Message <Items :: Character> >
		{
			public :
				static string get_class_name ();

				Conversation_Engine ();
				virtual ~Conversation_Engine ();
				virtual bool is_initialized () const;
				
				void list_options
				(
					Reference <Items :: Character> speaker,
					Reference <Items :: Character> listener
				);

			private :
				bool evaluate_condition
					(const TiXmlElement * element, Reference <Items :: Character> subject);
				bool evaluate_expression
					(const TiXmlAttribute * attribute, Reference <Items :: Character> subject);
			
				boost :: scoped_ptr <TiXmlDocument> behavior;
		};
	}
}

#endif	// RADAKAN_CONVERSATION_ENGINE_HPP
