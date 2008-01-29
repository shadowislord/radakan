#ifndef RADAKAN_SETTINGS_HPP
#define RADAKAN_SETTINGS_HPP

#include <OgreConfigFile.h>

#include "singleton.hpp"

namespace Radakan
{
	namespace Engines
	{

		///	Settings contains all out-of-character player settings.
		class Settings :
			public Singleton <Settings>
		{
			public :
				Settings (string);
				virtual ~Settings ();
				virtual bool is_initialized () const;
				
				static string get_class_name ();

				void load_key_bindings (string game_mode_name);

				///	'get_key_name' returns an empty string when no matching key is found.
				string get_key_name (string game_mode_name, string command) const;

				string radakan_path;
				string ogre_media_path;

				float movement_reaction;
				float maximal_movement_speed;
				float turn_reaction;
				float maximal_turn_speed;

			private :
				// For loading the radakan settings from a file
				boost :: scoped_ptr <Ogre :: ConfigFile> radakan_config;

				//	'key_bindings' maps a game_mode + command to a key name.
				map <string, string> key_bindings;
		};
	}
}

#endif	// RADAKAN_SETTINGS_HPP
