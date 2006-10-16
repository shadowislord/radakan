#ifndef PEACE_STATE_HPP
#define PEACE_STATE_HPP

#include "state_machine.hpp"

using namespace std;

class Peace_State:
	public State
{
	public:
		Peace_State (Character * new_owner, State * new_parent_state);
		virtual ~Peace_State ();
		virtual bool is_initialized () const;
		virtual void act ();
		virtual void think ();
};

#endif	//	PEACE_STATE_HPP
