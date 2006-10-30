#include "npc.hpp"

using namespace std;
using namespace sl;

//  constructor
NPC ::
	NPC (Ogre :: SceneNode * new_node) :
	Object (get_name (new_node)),
	Character (new_node),
	State_Machine (NULL)
{
	assert (Character :: is_initialized ());
	assert (State_Machine :: is_initialized ());
	
	assert (is_initialized ());
}

//  destructor
NPC ::
	~NPC ()
{
	assert (Object :: is_initialized (* this + " -> ~NPC ()"));
}

//	virtual
bool NPC ::
	is_initialized ()
	const
{
	return Character :: is_initialized () && State_Machine :: is_initialized ();
}
