/*#ifndef RADAKAN_CONVERSATION_PLUGIN_HPP
#define RADAKAN_CONVERSATION_PLUGIN_HPP

#include "character.hpp"
#include "play_state.hpp"

using namespace std;

namespace Radakan
{
	namespace Plugins
	{

		class Conversation_Plugin :
//			public Singleton <Conversation_Plugin>,
			public Observer <Strategies :: Play_State>
		{
			public :
				Conversation_Plugin ();
				virtual ~Conversation_Plugin ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual void call (const Object & message);

			private :
				TiXmlDocument behavior;
		};
	}
}

#endif	// RADAKAN_CONVERSATION_PLUGIN_HPP*/
