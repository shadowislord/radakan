#ifndef PEACE_STATE_HPP
#define PEACE_STATE_HPP

#include "state_machine.hpp"

using namespace std;

class Peace_State:
	public State
{
	public:
		Peace_State (NPC * new_owner);
		virtual ~Peace_State ();
		virtual bool is_initialized () const;
		virtual void act ();
		virtual void think (State * my_parent);
};

#endif	//	PEACE_STATE_HPP
