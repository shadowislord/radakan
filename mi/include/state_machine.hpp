#ifndef AI_MACHINE_HPP
#define AI_MACHINE_HPP

#include "state.hpp"

using namespace std;

class State_Machine:
	public State,
	public Tree
{
	public:
		State_Machine (Character * new_owner, State * new_parent_state);
		virtual ~State_Machine ();
		virtual bool is_initialized () const;
		virtual void act ();
		virtual void think ();
		template <typename T> void change_active_state ();
		virtual bool add (Object * sub_tree);
		template <typename T> void add ();

	private:
		State * active_child_state;
};

#include "state_machine.ipp"

#endif	//	AI_MACHINE_HPP
