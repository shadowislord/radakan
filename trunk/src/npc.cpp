#include "npc.hpp"

using namespace std;
using namespace sl;

//  constructor
NPC ::
	NPC (Ogre :: SceneNode & new_node) :
	Object (get_name (new_node)),
	Character (new_node),
	State_Machine ()
{
	trace () << "NPC (" << get_name (* node) << ")" << endl;
	assert (Character :: is_initialized ());
//	trace () << "NPC (" << get_name (* node) << ") A" << endl;
	assert (State_Machine :: is_initialized ());
//	trace () << "NPC (" << get_name (* node) << ") B" << endl;

	State_Machine :: add <Peace_State> ();
//	trace () << "NPC (" << get_name (* node) << ") C" << endl;
	State_Machine :: add <Fight_State> ();
//	trace () << "NPC (" << get_name (* node) << ") D" << endl;
	
	assert (is_initialized ());
//	trace () << "NPC (" << get_name (* node) << ") E" << endl;
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
	return warn <NPC> (Character :: is_initialized () && (is_dead () || State_Machine :: is_initialized ()));
}

//	static
string NPC ::
	get_type_name ()
{
	return "NPC";
}
