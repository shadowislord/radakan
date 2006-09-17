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

        virtual void Set_Event(char evt);
        virtual char Get_Event();
};

#endif	//	EVENT_HPP
