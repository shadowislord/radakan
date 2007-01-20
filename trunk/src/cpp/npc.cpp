#include "npc.hpp"

using namespace std;
using namespace tsl;

//  constructor
NPC ::
	NPC
	(
		float new_volume,
		float new_weight,
		btVector3 new_position,
		Ogre :: SceneNode & new_node
	) :
	Object (get_name (new_node)),
	Character
	(
		new_volume,
		new_weight,
		new_position,
		new_node
	),
	State_Machine <NPC> (* this)
{
	trace () << "NPC (" << get_name (* node) << ")" << endl;
	assert (Character :: is_initialized ());
	assert (State_Machine <NPC> :: is_initialized ());

	State_Machine <NPC> :: add <Peace_State> ();

	assert (is_initialized ());
}

//  destructor
NPC ::
	~NPC ()
{
	trace () << "~NPC ()" << endl;
	assert (Object :: is_initialized ());
}

//	virtual
bool NPC ::
	is_initialized ()
	const
{
	assert (warn <NPC> (Character :: is_initialized ()));
	assert (warn <NPC> (State_Machine <NPC> :: is_initialized ()));
	return true;
}

//	static
string NPC ::
	get_class_name ()
{
	return "NPC";
}

//	virtual
string NPC ::
	die ()
{
	assert (NPC :: is_initialized ());

	change_active_state <Dead_State> ();

	return Character :: die ();
}
