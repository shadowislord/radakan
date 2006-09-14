#ifndef FIGHT_STATE_HPP
#define FIGHT_STATE_HPP

#include "state_machine.hpp"

using namespace std;

class Fight_State:
	public State
{
	public:
		Fight_State (NPC * new_owner);
		virtual ~Fight_State ();
		virtual bool is_initialized () const;
		virtual void act ();
		virtual void think (State * my_parent);
};

#endif	//	FIGHT_STATE_HPP
