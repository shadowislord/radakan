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
	State_Machine <Character> (this -> to_type <Character> ())
{
	trace () << "NPC (" << get_name (* node) << ")" << endl;
	assert (Character :: is_initialized ());
//	trace () << "NPC (" << get_name (* node) << ") A" << endl;
	assert (State_Machine <Character> :: is_initialized ());
//	trace () << "NPC (" << get_name (* node) << ") B" << endl;

	State_Machine <Character> :: add <Peace_State> ();
//	trace () << "NPC (" << get_name (* node) << ") C" << endl;

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
	assert (warn <NPC> (Character :: is_initialized ()));
	assert (warn <NPC> (State_Machine <Character> :: is_initialized ()));
	return true;
}

//	static
string NPC ::
	get_type_name ()
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
