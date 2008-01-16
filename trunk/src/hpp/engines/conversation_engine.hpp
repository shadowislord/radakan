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

	namespace Strategies
	{
		namespace Behaviors
		{
			class AI;
		}
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
				
				void list_player_options ();

			private :
				void load_player_options
					(const TiXmlElement * element,
						Reference <Strategies :: Behaviors :: AI> ai);
				bool evaluate_npc_condition
					(const TiXmlElement * element,
						Reference <Strategies :: Behaviors :: AI> ai);
				bool evaluate_npc_expression
					(const TiXmlAttribute * attribute,
						Reference <Strategies :: Behaviors :: AI> ai);
				bool evaluate_player_character_condition (const TiXmlElement * element);
				bool evaluate_player_character_expression (const TiXmlAttribute * attribute);
			
				boost :: scoped_ptr <TiXmlDocument> behavior;
		};
	}
}

#endif	// RADAKAN_CONVERSATION_ENGINE_HPP
