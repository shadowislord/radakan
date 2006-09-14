#ifndef NPC_HPP
#define NPC_HPP

#include "state.hpp"

using namespace std;

class NPC:
	public Character
{
	public:
		NPC
			(Ogre :: Entity * new_ogre_entity,
			Ogre :: SceneNode * new_node);
		virtual ~NPC ();
		virtual bool is_initialized () const;
		State * ai;
		virtual void change_active_state (State * new_state);
};

#endif	//	NPC_HPP
