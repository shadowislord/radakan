#ifndef FIGHT_STATE_HPP
#define FIGHT_STATE_HPP

#include "state_machine.hpp"

using namespace std;

class Fight_State:
	public State
{
	public:
		Fight_State (Character * new_owner, State * new_parent_state);
		virtual ~Fight_State ();
		virtual bool is_initialized () const;
		virtual void act ();
		virtual void think ();
};

#endif	//	FIGHT_STATE_HPP
