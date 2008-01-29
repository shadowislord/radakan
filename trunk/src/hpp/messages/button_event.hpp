#ifndef RADAKAN_MESSAGES_BUTTON_EVENT_HPP
#define RADAKAN_MESSAGES_BUTTON_EVENT_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	namespace Messages
	{

		///	TODO Add documentation.
		class Button_Event :
			virtual public Object
		{
			public :
				static string get_class_name ();

				Button_Event (string new_name);
				virtual ~Button_Event ();
				virtual bool is_initialized () const;
		};
	}
}

#endif	// RADAKAN_MESSAGES_BUTTON_EVENT_HPP
