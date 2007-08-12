#include "alive_state.hpp"
#include "log.hpp"
#include "npc.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Items;

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
	Engines :: Log :: trace (me, NPC :: get_class_name (), "", new_name, new_mesh_name, to_string (new_size), to_string (new_mass));

	set_active_state (static_cast <Strategies :: Strategy &> (* new Strategies :: Alive_State (* this)));

	assert (is_initialized ());
	Engines :: Log :: log (me) << "I'm fully constructed (as NPC)." << endl;
}

//  destructor
NPC ::
	~NPC ()
{
	Engines :: Log :: trace (me, NPC :: get_class_name (), "~");
	assert (NPC :: is_initialized ());

	prepare_for_destruction ();
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
void NPC ::
	call (const Object & message)
{
	assert (is_initialized ());

	Strategies :: Strategy_State_Machine :: run (message);
}

//	virtual
bool NPC ::
	is_dead () const
{
	return ! has_active_state ();
}

//	virtual
void NPC ::
	die ()
{
	assert (NPC :: is_initialized ());
	assert (has_model ());

	call (terminate);

	assert (is_dead ());
}

//	virtual
void NPC ::
	drop (Object & t, bool stay)
{
	Engines :: Log :: trace (this -> me, NPC :: get_class_name (), "drop", t . name, bool_to_string (stay));
	assert (NPC :: is_initialized ());

	if (t . is_type <Strategies :: Strategy> ())
	{
		Strategies :: Strategy_State_Machine :: drop (t, stay);
	}
	else
	{
		Container :: drop (t, stay);
	}
}

void NPC ::
	think (const string tought)
	const
{
	Engines :: Log :: show (name + " thinks \"" + tought + "\"");
}
