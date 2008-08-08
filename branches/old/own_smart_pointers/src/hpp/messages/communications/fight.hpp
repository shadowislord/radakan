#ifndef RADAKAN_MESSAGES_COMMUNICATIONS_FIGHT_HPP
#define RADAKAN_MESSAGES_COMMUNICATIONS_FIGHT_HPP

#include "messages/communications/communication.hpp"

using namespace std;

namespace Radakan
{
	namespace Messages
	{
		namespace Communications
		{
			class Fight :
				public Communication
			{
				public :
					static string get_class_name ();
					
					Fight
					(
						const string & information,
						Reference <Items :: Character> new_from,
						Reference <Items :: Character> new_to
					);
					virtual ~Fight ();
					virtual bool is_initialized () const;
					
					virtual string get_communication_name ();
			};
		}
	}
}

#endif	// RADAKAN_MESSAGES_COMMUNICATIONS_FIGHT_HPP
