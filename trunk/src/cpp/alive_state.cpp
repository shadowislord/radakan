#include "alive_state.hpp"
#include "fight_state.hpp"
#include "npc.hpp"
#include "tsl.hpp"

using namespace std;
using namespace tsl;

//  constructor
Alive_State ::
	Alive_State () :
	Object ("alive state")
{
	assert (Algorithm <NPC> :: is_initialized ());

	assert (Alive_State :: is_initialized ());
}

//  destructor
Alive_State ::
	~Alive_State ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;
	
	assert (Algorithm <NPC> :: is_initialized ());
}

//	virtual
bool Alive_State ::
	is_initialized ()
	const
{
	assert (Algorithm <NPC> :: is_initialized ());
	
	return true;
}

//	static
string Alive_State ::
	get_class_name ()
{
	return "Alive_State";
}

//	virtual
Algorithm <NPC> & Alive_State ::
	transit (NPC & owner)
{
	assert (is_initialized ());
	
	const float turn_lenght = TSL :: get () . get_last_turn_lenght ();
	
	owner . get_body () . move (0, turn_lenght);
	owner . get_body () . turn (0, turn_lenght);
	
	if (! owner . hands . is_empty ())
	{
		return Fight_State :: get ();
	}

	return owner . get_active_state ();
}
