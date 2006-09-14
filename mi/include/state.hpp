#ifndef STATE_HPP
#define STATE_HPP

#include "character.hpp"

using namespace std;

class State:
	public virtual Object
{
	public:
		State (Character * new_owner);
		virtual ~State ();
		virtual bool is_initialized () const;
		virtual void act ();
		virtual void think (State * my_parent);	//	change your current state
		virtual void change_active_state (State * new_state);
		Character * owner;
};

#endif	//	STATE_HPP
