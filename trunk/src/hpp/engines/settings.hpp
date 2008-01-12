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

				///	in meters
				const float & get_camera_distance () const;
				
				///	in meters
				void increase_camera_distance (float distance);
				
				///	in radians
				const float & get_vertical_camera_angle () const;
				
				///	in radians
				void increase_vertical_camera_angle (float angle);

				// get the config object
				Ogre::ConfigFile* getRadakanConfig();

				string radakan_path;

				string ogre_media_path;
				
				float movement_speed;

				float turn_speed;

				string forward_key;	

				string backward_key;

				string left_key;

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

				// For loading the radakan settings from a file
				Ogre::ConfigFile *radakan_config;
		};
	}
}

#endif	// RADAKAN_SETTINGS_HPP
