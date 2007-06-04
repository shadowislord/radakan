#include "log.hpp"
#include "settings.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Engines;

//	static
const string Settings ::
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
	Settings (string new_tsl_path) :
	Object ("settings"),
	tsl_path (new_tsl_path),
	camera_distance (0.72)
{
	//	Do nothing.
	
	assert (is_initialized ());
}

Settings ::
	~Settings ()
{
	Log :: trace (me, Settings :: get_class_name (), "~");
	assert (is_initialized ());

	forget_dependencies ();
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
