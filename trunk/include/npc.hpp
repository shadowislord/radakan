#ifndef NPC_HPP
#define NPC_HPP

#include "state_machine.hpp"

using namespace std;

class NPC:
	public Character,
	public State_Machine
{
	public:
		NPC
			(Ogre :: Entity * new_ogre_entity,
			Ogre :: SceneNode * new_node);
		virtual ~NPC ();
		virtual bool is_initialized () const;

};

#endif	//	NPC_HPP
