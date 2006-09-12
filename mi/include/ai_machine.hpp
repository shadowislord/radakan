#ifndef AI_MACHINE_HPP
#define AI_MACHINE_HPP

#include "ai_state.hpp"
#include "tree.hpp"

using namespace std;

class AI_Machine:
	public AI_State,
	public Tree
{
	public:
		AI_Machine (string new_name);
		virtual ~AI_Machine ();
		virtual bool is_initialized () const;
		virtual bool add (Object * sub_tree);
};

#endif	//	AI_MACHINE_HPP
