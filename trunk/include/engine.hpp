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
		
		Event * process (Event * event);
};

#endif	//	ENGINE_HPP
