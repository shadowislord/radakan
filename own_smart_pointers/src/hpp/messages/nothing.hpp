#ifndef RADAKAN_MESSAGES_NOTHING_HPP
#define RADAKAN_MESSAGES_NOTHING_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	namespace Messages
	{

		///	'Nothing' should never be instantiatiated!
		class Nothing :
			public Object
		{
			public :
				static string get_class_name ();

				///	Returns an empty reference.
				static Reference <Nothing> get ();

				///	A destructor is of no use, but it's added for consystency.
				virtual ~Nothing ();
				///	'is_initialized' is of no use, but it's added for consystency.
				virtual bool is_initialized () const;

			private :
				Nothing ();
		};
	}
}

#endif	// RADAKAN_MESSAGES_NOTHING_HPP
