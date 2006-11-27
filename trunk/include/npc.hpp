#ifndef NPC_HPP
#define NPC_HPP

#include "peace_state.hpp"
#include "fight_state.hpp"

using namespace std;

namespace sl
{
	class NPC :
		public Character,
		public State_Machine
	{
		public :
			NPC (Ogre :: SceneNode & new_node);
			virtual ~NPC ();
			virtual bool is_initialized () const;
			static string get_type_name ();
	};
}

#endif	//	NPC_HPP
