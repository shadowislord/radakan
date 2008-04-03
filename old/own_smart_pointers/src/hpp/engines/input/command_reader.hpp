#ifndef RADAKAN_ENGINES_INPUT_COMMAND_READER_HPP
#define RADAKAN_ENGINES_INPUT_COMMAND_READER_HPP

#include "singleton.hpp"

using namespace std;

namespace Radakan
{
	namespace Engines
	{
		namespace Input
		{

			///	TODO Add comment.
			class Command_Reader :
				public Singleton <Command_Reader>
			{
				public :
					static string get_class_name ();

					Command_Reader ();
					virtual ~Command_Reader ();
					virtual bool is_initialized () const;

					string get_communication_option ();
					bool has_command
						(string caller_class, string command_name, bool reset = true);
			};
		}
	}
}

#endif	// RADAKAN_ENGINES_INPUT_COMMAND_READER_HPP
