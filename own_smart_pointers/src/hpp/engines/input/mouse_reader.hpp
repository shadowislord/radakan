#ifndef RADAKAN_ENGINES_INPUT_MOUSE_READER_HPP
#define RADAKAN_ENGINES_INPUT_MOUSE_READER_HPP

#include "singleton.hpp"

using namespace std;

namespace Radakan
{
	namespace Engines
	{
		namespace Input
		{

			///	TODO Add comment.
			class Mouse_Reader :
				public Singleton <Mouse_Reader>
			{
				public :
					static string get_class_name ();

					static string get_left_mouse_button ();
					static string get_middle_mouse_button ();
					static string get_right_mouse_button ();

					Mouse_Reader ();
					virtual ~Mouse_Reader ();
					virtual bool is_initialized () const;

					bool is_mouse_button_pressed (string button, bool reset);
					const Mathematics :: Vector_3D & get_absolute_mouse_position () const;
					const Mathematics :: Vector_3D & get_relative_mouse_position () const;
			};
		}
	}
}

#endif	// RADAKAN_ENGINES_INPUT_MOUSE_READER_HPP
