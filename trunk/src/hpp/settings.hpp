#ifndef TSL_SETTINGS_HPP
#define TSL_SETTINGS_HPP

#include "singleton.hpp"

// A quick way to enable WSAD is to define TSL_WSAD.
#ifdef TSL_WIN
	#define TSL_WSAD
#endif

namespace TSL
{
	namespace Engines
	{

		///	Settings contains all out-of-character player settings.
		class Settings :
			public Singleton <Settings>
		{
			public :
				Settings (string new_tsl_path);
				virtual ~Settings ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				///	in meters
				const float & get_camera_distance () const;
				
				///	in meters
				void increase_camera_distance (float distance);
				
				///	in radians
				const float & get_vertical_camera_angle () const;
				
				///	in radians
				void increase_vertical_camera_angle (float angle);

				const string tsl_path;

				///	Defaults to e, unless TSL_WSAD is set.
				string forward_key;	

				///	Defaults to d, unless TSL_WSAD is set.
				string backward_key;

				///	Defaults to s, unless TSL_WSAD is set.
				string left_key;

				///	Defaults to f, unless TSL_WSAD is set.
				string right_key;

			private :
				///	in meters
				float camera_distance;

				///	in radians
				static const float min_vertical_camera_angle;
				
				///	in radians
				static const float max_vertical_camera_angle;

				///	in radians
				float vertical_camera_angle;
		};
	}
}

#endif	// TSL_SETTINGS_HPP
