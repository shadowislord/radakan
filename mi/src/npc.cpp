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
		new_ogre_entity->getName ()),
	Character (new_ogre_entity, new_node)
{
	assert (Character :: is_initialized ());
	
	ai = NULL;
	
	assert (is_initialized ());
}

//  destructor
NPC ::
	~NPC ()
{
	assert (Object :: is_initialized (* this + "->~NPC ()"));

	if (ai != NULL)
	{
		delete ai;
	}
}

//	virtual
bool NPC ::
	is_initialized ()
	const
{
	return Character :: is_initialized () && ((ai == NULL) || ai->is_initialized ());
}

//	virtual
void NPC ::
	change_active_state (State * new_state)
{
	assert (is_initialized ());
	assert (ai != NULL);

	ai->change_active_state (new_state);
}
