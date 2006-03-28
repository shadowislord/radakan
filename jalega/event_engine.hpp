#ifndef EVENT_ENGINE_HPP
#define EVENT_ENGINE_HPP 

#include "input_event.hpp"

using namespace std;

typedef void (* void_func) ();

class Event_Engine:
	public Object
{
	public:
		Event_Engine (void_func new_quit, void_func new_refresh);
		virtual ~Event_Engine ();
		void process (Input_Event * event);
	
	private:
		void_func refresh;
		void_func quit;
};

extern int font_index;

#endif
