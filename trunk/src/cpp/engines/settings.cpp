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

//	static
const float Settings ::
	min_vertical_camera_angle (- Ogre :: Math :: HALF_PI);

//	static
const float Settings ::
	max_vertical_camera_angle (Ogre :: Math :: HALF_PI);

Settings ::
	Settings (string path_to_config) :
	Object ("settings", true),	//	Here 'true' means 'prevent automatic destruction'.
	camera_distance (0.72),
	vertical_camera_angle (0),
	radakan_config (new Ogre :: ConfigFile ())
{
	//	Load the Radakan configuration file
	radakan_config -> load (path_to_config + "/radakan.cfg");

	radakan_path = radakan_config -> getSetting ("radakan_path", "directories");
	ogre_media_path = radakan_config -> getSetting ("ogre_media_path", "directories");

	forward_key = radakan_config -> getSetting ("forward", "keyboard");
	backward_key = radakan_config -> getSetting ("backward", "keyboard");
	left_key = radakan_config -> getSetting ("left", "keyboard");
	right_key = radakan_config -> getSetting ("right", "keyboard");

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

const float & Settings ::
	get_camera_distance ()
	const
{
	assert (is_initialized ());

	return camera_distance;
}

void Settings ::
	increase_camera_distance (float distance)
{
	assert (is_initialized ());

	camera_distance += distance;
}

const float & Settings ::
	get_vertical_camera_angle ()
	const
{
	assert (is_initialized ());

	return vertical_camera_angle;
}

void Settings ::
	increase_vertical_camera_angle (float angle)
{
	assert (is_initialized ());

	vertical_camera_angle += angle;

	if (vertical_camera_angle < min_vertical_camera_angle)
	{
		vertical_camera_angle = min_vertical_camera_angle;
	}
	else if (max_vertical_camera_angle < vertical_camera_angle)
	{
		vertical_camera_angle = max_vertical_camera_angle;
	}
}
