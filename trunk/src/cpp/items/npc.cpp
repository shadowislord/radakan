#include "engines/log.hpp"
#include "items/npc.hpp"
#include "messages/battle_message.hpp"
#include "messages/conversation_message.hpp"
#include "opinion.hpp"
#include "strategies/alive_state.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Items;

//	static
string NPC ::
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
	),
	opinions (me . get_name () + "'s opinions")
{
	Engines :: Log :: trace (me, NPC :: get_class_name (), "", me . get_name (), new_mesh_name, to_string (new_size), to_string (new_mass));

	set_active_state (Reference <Strategies :: Strategy <Character> >
		(new Strategies :: Alive_State (Reference <NPC> (this))));

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
	call (const Reference <Messages :: Message <Character> > & message)
{
	assert (is_initialized ());

	Strategies :: Strategy_State_Machine <Character> :: run (message);
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
	Engines :: Log :: trace (this -> me, NPC :: get_class_name (), "die");
	assert (NPC :: is_initialized ());
	assert (has_model ());

	call (Messages :: Message <Character> :: terminate);

	assert (is_dead ());
}
