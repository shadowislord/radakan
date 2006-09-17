#ifndef MOVE_EVENT_HPP
#define MOVE_EVENT_HPP

#include "event.hpp"

using namespace std;

class Move_Event:
	public virtual Event
{
	public:
		Move_Event (string direction);
		virtual ~Move_Event ();
		virtual bool is_initialized () const;
		bool is_direction (string direction) const;
		static const string forward;
		static const string backward;
		static const string left;
		static const string right;
};

#endif	//	MOVE_EVENT_HPP
