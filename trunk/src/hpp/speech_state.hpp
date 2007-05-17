#ifndef TSL_SPEECH_STATE_HPP
#define TSL_SPEECH_STATE_HPP

#include "chat_state.hpp"

using namespace std;

namespace TSL
{
	class NPC;

	class Speech_State :
		public Algorithm <NPC>
	{
		public :
			virtual ~Speech_State ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();
			
			virtual Algorithm <NPC> & transit (NPC & owner);

			static Algorithm <NPC> & create (string new_message);

		private :
			Speech_State (string new_message);
	};
}

#endif	//	TSL_SPEECH_STATE_HPP
