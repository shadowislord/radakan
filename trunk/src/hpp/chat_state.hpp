#ifndef TSL_CHAT_STATE_HPP
#define TSL_CHAT_STATE_HPP

#include "algorithm.hpp"

using namespace std;

namespace TSL
{
	class NPC;

	class Chat_State :
		public Singleton <Chat_State>,
		public Algorithm <NPC>
	{
		public :
			Chat_State ();
			virtual ~Chat_State ();
			virtual bool is_initialized () const;
			
			static const string class_name;
			
			virtual Algorithm <NPC> & transit (NPC & owner);
	};
}

#endif	//	TSL_CHAT_STATE_HPP
