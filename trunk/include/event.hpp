#ifndef EVENT_HPP
#define EVENT_HPP

#include "object.hpp"

using namespace std;

class Event:
	public virtual Object
{
	public:
		Event (string new_name);
		virtual ~Event ();
		virtual bool is_initialized () const;

		virtual void set_event (char evt);
		virtual char get_event ();
};

#endif	//	EVENT_HPP
