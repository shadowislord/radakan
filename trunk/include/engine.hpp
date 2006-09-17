#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "event.hpp"

using namespace std;

class Engine:
	public Object
{
	public:
		Engine (string new_name);
		virtual ~Engine ();
		virtual bool is_initialized () const;

		virtual Event * process (Event * event);

		queue <Event*> event_queue;
};

#endif	//	ENGINE_HPP
