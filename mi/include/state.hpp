#ifndef STATE_HPP
#define STATE_HPP

#include "character.hpp"

using namespace std;

class State:
	public virtual Object
{
	public:
		State (Character * new_owner, State * new_parent_state);
		virtual ~State ();
		virtual bool is_initialized () const;
		virtual void act ();
		virtual void think ();	//	change your current state
		Character * owner;
		State * parent_state;
};

#endif	//	STATE_HPP
