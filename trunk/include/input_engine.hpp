
#ifndef INPUT_ENGINE_HPP
#define INPUT_ENGINE_HPP

#include "event.hpp"
#include "engine.hpp"

using namespace std;

///	Input engine is responsible for translating player input into events and actions.

class Input_Engine:
	public Engine
{
	public:
		Input_Engine ();
		virtual ~Input_Engine ();
		virtual bool is_initialized () const;
		virtual Event * process (Event * event);

	private:
};

#endif	//	INPUT_ENGINE_HPP
