#include "npc.hpp"
#include "alive_state.hpp"
#include "dead_state.hpp"

using namespace std;
using namespace TSL;

//	static
const string NPC ::
	class_name ("NPC");

//  constructor
NPC ::
	NPC
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass
	) :
	Object (new_name),
	Character
	(
		new_mesh_name,
		new_size,
		new_mass
	)
{
	log (debugging) << class_name << " (" << new_name << ", ...)" << endl;
	assert (Character :: is_initialized ());

	set_active_state (Alive_State :: get ());

	assert (is_initialized ());
	log (debugging) << "I'm fully constructed (as NPC)." << endl;
}

//  destructor
NPC ::
	~NPC ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
	assert (NPC :: is_initialized ());
}

//	virtual
bool NPC ::
	is_initialized ()
	const
{
	assert (Character :: is_initialized ());
	return true;
}

//	virtual
bool NPC ::
	is_dead () const
{
	return get_active_state () . is_type <Dead_State> ();
}

//	virtual
void NPC ::
	die ()
{
	assert (NPC :: is_initialized ());
	assert (has_body ());

	set_active_state (Dead_State :: get ());
}

//	static
Item & NPC ::
	create
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass
	)
{
	Item * temp =
		new NPC
		(
			new_name,
			new_mesh_name,
			new_size,
			new_mass
		);

	return * temp;
}
