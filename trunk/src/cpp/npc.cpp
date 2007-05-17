#include "alive_state.hpp"
#include "dead_state.hpp"
#include "log.hpp"
#include "npc.hpp"

using namespace std;
using namespace TSL;

//	static
const string NPC ::
	get_class_name ()
{
	return "NPC";
}

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
	Log :: trace <NPC> (me, "", new_name, new_mesh_name, to_string (new_size), to_string (new_mass));
	assert (Character :: is_initialized ());

	set_active_state (Alive_State :: get ());

	assert (is_initialized ());
	Log :: log (me) << "I'm fully constructed (as NPC)." << endl;
}

//  destructor
NPC ::
	~NPC ()
{
	Log :: trace <NPC> (me, "~");
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
	assert (has_model ());

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
