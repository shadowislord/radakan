#ifndef NPC_HPP
#define NPC_HPP

#include "state_machine.hpp"

using namespace std;

namespace sl
{
	class NPC :
		public Character,
		public State_Machine
	{
		public :
			NPC
				(Ogre :: SceneNode * new_node);
			virtual ~NPC ();
			virtual bool is_initialized () const;

	};
}

#endif	//	NPC_HPP
