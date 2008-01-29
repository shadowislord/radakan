#ifndef RADAKAN_MESSAGES_LIST_EVENT_HPP
#define RADAKAN_MESSAGES_LIST_EVENT_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	namespace Messages
	{

		///	TODO Add documentation.
		class List_Event :
			public Object
		{
			public :
				static string get_class_name ();

				List_Event (string new_name);
				virtual ~List_Event ();
				virtual bool is_initialized () const;
		};
	}
}

#endif	// RADAKAN_MESSAGES_LIST_EVENT_HPP
