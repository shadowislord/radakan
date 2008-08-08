#ifndef RADAKAN_MESSAGES_COMMUNICATION_CONVERSE_HPP
#define RADAKAN_MESSAGES_COMMUNICATION_CONVERSE_HPP

#include "messages/communications/communication.hpp"

using namespace std;

class TiXmlElement;

namespace Radakan
{
	namespace Messages
	{
		namespace Communications
		{

			///	TODO Add comment.
			class Converse :
				public Communication
			{
				public :
					static string get_class_name ();

					Converse
					(
						Reference <Items :: Character> new_from,
						Reference <Items :: Character> new_to,
						const TiXmlElement * new_option
					);
					virtual ~Converse ();
					virtual bool is_initialized () const;

					virtual string get_communication_name ();
					
					Reference <Communication> get_reaction () const;

					///	'option' is a reference to an XML element in the behavior file.
					const TiXmlElement * option;

				private :
					static string create_name (const TiXmlElement * option);
			};
		}
	}
}

#endif	// RADAKAN_MESSAGES_COMMUNICATION_CONVERSE_HPP
