#ifndef RADAKAN_ENGINES_INPUT_MOUSE_DATA_HPP
#define RADAKAN_ENGINES_INPUT_MOUSE_DATA_HPP

#include "singleton.hpp"

using namespace std;

namespace Radakan
{
	namespace Engines
	{
		namespace Input
		{
		
			///	TODO Add comment.
			class Mouse_Data :
				public Singleton <Mouse_Data>
			{
				public :
					static string get_class_name ();
					static bool is_mouse_button (string button);

					static const string left_mouse_button;
					static const string middle_mouse_button;
					static const string right_mouse_button;
					
					Mouse_Data ();
					virtual ~Mouse_Data ();
					virtual bool is_initialized () const;

					///	Multiple mouse buttons can be pressed at once.
					set <string> pressed_mouse_buttons;

					Ogre :: Vector3 relative_mouse_position;
					Ogre :: Vector3 absolute_mouse_position;
			};
		}
	}
}

#endif	// RADAKAN_ENGINES_INPUT_MOUSE_DATA_HPP
