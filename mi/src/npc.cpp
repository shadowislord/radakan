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

	delete ai;
}

//	virtual
bool NPC ::
	is_initialized ()
	const
{
	return Character :: is_initialized () && ((ai == NULL) || ai->is_initialized ());
}

void NPC ::
	create_ai ()
{
	assert (is_initialized ());
	assert (ai == NULL);
	
	ai = new State_Machine (this, NULL);

	assert (is_initialized ());
}
