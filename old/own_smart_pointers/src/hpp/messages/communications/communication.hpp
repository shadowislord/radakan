#ifndef RADAKAN_MESSAGES_COMMUNICATIONS_COMMUNICATION_HPP
#define RADAKAN_MESSAGES_COMMUNICATIONS_COMMUNICATION_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{
		class Character;
	}

	namespace Messages
	{
		namespace Communications
		{
			class Communication :
				public Object
			{
				public :
					static string get_class_name ();

					Communication
					(
						string new_name,
						Reference <Items :: Character> new_from,
						Reference <Items :: Character> new_to
					);
					virtual ~Communication ();
					virtual bool is_initialized () const;

					///	'get_action_name ()' returns the name of a subclass.
					virtual string get_communication_name () = 0;

					Reference <Items :: Character> from;
					Reference <Items :: Character> to;
			};
		}
	}
}

#endif	// RADAKAN_MESSAGES_COMMUNICATIONS_COMMUNICATION_HPP
