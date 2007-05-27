#ifndef TSL_CONVERSATION_PLUGIN_HPP
#define TSL_CONVERSATION_PLUGIN_HPP

#include "character.hpp"
#include "play_state.hpp"

using namespace std;

namespace TSL
{
	namespace Plugins
	{

		class Conversation_Plugin :
			public Singleton <Conversation_Plugin>,
			public Observer <Algorithms :: Play_State>
		{
			public :
				Conversation_Plugin ();
				virtual ~Conversation_Plugin ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual void call (const Object & message);

			private :
				void add (Items :: Character & one, Items :: Character & two);
		};
	}
}

#endif	// TSL_CONVERSATION_PLUGIN_HPP
