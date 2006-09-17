#ifndef EXIT_EVENT_HPP
#define EXIT_EVENT_HPP

#include "event.hpp"

using namespace std;

class Exit_Event:
	public virtual Event
{
	public:
		Exit_Event ();
		virtual ~Exit_Event ();
		virtual bool is_initialized () const;
};

#endif	//	EXIT_EVENT_HPP
