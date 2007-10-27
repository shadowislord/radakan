#ifndef RADAKAN_MESSAGE_HPP
#define RADAKAN_MESSAGE_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	namespace Messages
	{

		///	This is an base class for all message classes.
		template <class T> class Message :
			virtual public Object
		{
			public :
				static string get_class_name ();

				static const Reference <Message <T> > update;
				static const Reference <Message <T> > terminate;

				Message
				(
					string new_name,
					Reference <T> new_from = Reference <T> (),
					Reference <T> new_to = Reference <T> ()
				);
				virtual ~Message ();
				virtual bool is_initialized () const;

				///	'from' can refer to 'NULL'.
				Reference <T> from;

				///	'to' can refer to 'NULL'.
				Reference <T> to;
		};
	}
}

#endif	// RADAKAN_MESSAGE_HPP
