#include "npc.hpp"
#include "peace_state.hpp"
#include "dead_state.hpp"

using namespace std;
using namespace tsl;

//  constructor
NPC ::
	NPC
	(
		string new_name,
		string mesh_name,
		float new_volume,
		float new_mass
	) :
	Object (new_name),
	Character
	(
		mesh_name,
		new_volume,
		new_mass
	)
{
	trace () << get_class_name () << " (" << new_name << ")" << endl;
	assert (Character :: is_initialized ());

	set_active_state <Peace_State> ();

	assert (is_initialized ());
	trace () << " is fully constructed (as NPC)." << endl;
}

//  destructor
NPC ::
	~NPC ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (NPC :: is_initialized ());
}

//	virtual
bool NPC ::
	is_initialized ()
	const
{
	assert (warn <NPC> (Character :: is_initialized ()));
	return true;
}

//	static
string NPC ::
	get_class_name ()
{
	return "NPC";
}

//	virtual
bool NPC ::
	is_dead () const
{
	return get_active_state () . is_type <Dead_State> ();
}

//	virtual
string NPC ::
	die ()
{
	assert (NPC :: is_initialized ());

	set_active_state <Dead_State> ();

	return * this + " died!";
}

//	static
Item & NPC ::
	create
	(
		string new_name,
		string new_mesh_name,
		float new_volume,
		float new_mass
	)
{
	Item * temp =
		new NPC
		(
			new_name,
			new_mesh_name,
			new_volume,
			new_mass
		);

	return * temp;
}
