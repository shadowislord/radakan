#include <sstream>

#include "engines/log.hpp"
#include "engines/settings.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;

//	static
string Settings ::
	get_class_name ()
{
	return "Settings";
}

Settings ::
	Settings (string path_to_config) :
	Object ("settings", "singleton"),
	radakan_config (new Ogre :: ConfigFile ())
{
	//	Load the Radakan configuration file
	radakan_config -> load (path_to_config + "/radakan.cfg");

	radakan_path = radakan_config -> getSetting ("radakan_path", "directories");
	ogre_media_path = radakan_config -> getSetting ("ogre_media_path", "directories");
	
	//	Load the string values and convert them from string to float.
	movement_reaction
		= to_float (radakan_config -> getSetting ("movement_reaction", "gameplay"));
	maximal_movement_speed
		= to_float (radakan_config -> getSetting ("maximal_movement_speed", "gameplay"));
	turn_reaction
		= to_float (radakan_config -> getSetting ("turn_reaction", "gameplay"));
	maximal_turn_speed
		= to_float (radakan_config -> getSetting ("maximal_turn_speed", "gameplay"));

	assert (is_initialized ());
}

Settings ::
	~Settings ()
{
	Log :: trace (me, Settings :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Settings ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

void Settings ::
	load_key_bindings (string game_mode_name)
{
	assert (is_initialized ());
	
	Ogre :: ConfigFile :: SettingsIterator keys_iterator
		= radakan_config -> getSettingsIterator ("key bindings: " + game_mode_name);
	while (keys_iterator . hasMoreElements ())
	{
		key_bindings [game_mode_name + " - " + keys_iterator . peekNextKey ()]
			= keys_iterator . peekNextValue ();
		keys_iterator . moveNext ();
	}
}

string Settings ::
	get_key_name (string game_mode_name, string command)
	const
{
	assert (is_initialized ());

	map <string, string> :: const_iterator result
		= key_bindings . find (game_mode_name + " - " + command);

	if (result == key_bindings . end ())
	{
		return "";
	}
	else
	{
		return result -> second;
	}
}
