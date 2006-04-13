#ifndef EVENT_ENGINE_HPP
#define EVENT_ENGINE_HPP 

#include "character.hpp"
#include "input_event.hpp"

using namespace std;

class Event_Engine:
	public Object
{
	public:
		Event_Engine (void_func new_quit);
		virtual ~Event_Engine ();
		void process (Input_Event * event);
	
	private:
		void_func quit;
};

#endif

extern SHR * shr;
