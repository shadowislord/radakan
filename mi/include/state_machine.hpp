#ifndef AI_MACHINE_HPP
#define AI_MACHINE_HPP

#include "npc.hpp"
#include "tree.hpp"

using namespace std;

class State_Machine:
	public State,
	public Tree
{
	public:
		State_Machine (NPC * new_owner);
		virtual ~State_Machine ();
		virtual bool is_initialized () const;
		virtual void act ();
		virtual void think (State * my_parent);
		virtual void change_active_state (State * new_state);
		virtual bool add (Object * sub_tree);

	private:
		State * active_state;
};

#endif	//	AI_MACHINE_HPP
