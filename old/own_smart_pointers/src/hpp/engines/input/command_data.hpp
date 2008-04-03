#ifndef RADAKAN_ENGINES_INPUT_COMMAND_DATA_HPP
#define RADAKAN_ENGINES_INPUT_COMMAND_DATA_HPP

#include "singleton.hpp"

using namespace std;

namespace Radakan
{
	namespace Engines
	{
		namespace Input
		{
		
			///	TODO Add comment.
			class Command_Data :
				public Singleton <Command_Data>
			{
				public :
					static string get_class_name ();

					Command_Data ();
					virtual ~Command_Data ();
					virtual bool is_initialized () const;

					///	Multiple keys can be pressed at once.
					set <string> pressed_keys;

					///	Only one gui button can be clicked at once.
					string clicked_button;

					///	Only one communication option can be selected at once.
					string selected_communication_option;
			};
		}
	}
}

#endif	// RADAKAN_ENGINES_INPUT_COMMAND_DATA_HPP
