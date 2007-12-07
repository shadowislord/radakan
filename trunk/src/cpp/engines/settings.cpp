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
	Settings (string new_radakan_path) :
	Object ("settings"),
	radakan_path (new_radakan_path),
	camera_distance (0.72),
	vertical_camera_angle(0)
{
	#ifdef RADAKAN_WSAD
		forward_key = "w";
		backward_key = "s";
		left_key = "a";
		right_key = "d";
	#else
		forward_key = "e";
		backward_key = "d";
		left_key = "s";
		right_key = "f";
	#endif

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
