#include "npc.hpp"

using namespace std;

//  constructor
NPC ::
	NPC
		(Ogre :: Entity * new_ogre_entity,
		Ogre :: SceneNode * new_node) :
	Object
		((new_ogre_entity == NULL) ?
		"[ERROR: new_ogre_entity is NULL]" :
		new_ogre_entity -> getName ()),
	Character (new_ogre_entity, new_node),
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
