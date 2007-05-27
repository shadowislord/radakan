#ifndef TSL_CHAT_STATE_HPP
#define TSL_CHAT_STATE_HPP

#include "alive_state.hpp"
#include "gui.hpp"

using namespace std;

namespace TSL
{
	namespace Items
	{
		class NPC;
	}

	namespace Algorithms
	{

		///	Chat_State is the basic algorithm for conversating NPCs.
		class Chat_State :
			public Singleton <Chat_State>,
			public Algorithm <Items :: NPC>
		{
			public :
				Chat_State ();
				virtual ~Chat_State ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				virtual Algorithm <Items :: NPC> & transit (Items :: NPC & owner, const Object & message);

			private :
				virtual void enter (Items :: NPC & owner);
				
				virtual void exit (Items :: NPC & owner);
		};
	}
}

#endif	//	TSL_CHAT_STATE_HPP
