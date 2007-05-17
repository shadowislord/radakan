#include "alive_state.hpp"
#include "fight_state.hpp"
#include "game.hpp"
#include "log.hpp"
#include "npc.hpp"

using namespace std;
using namespace TSL;

//	static
const string Alive_State ::
	get_class_name ()
{
	return "Alive_State";
}

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
	Log :: trace <Alive_State> (me, "~");
	
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

//	virtual
Algorithm <NPC> & Alive_State ::
	transit (NPC & owner)
{
	assert (is_initialized ());
	
	const float turn_lenght = Game :: get () . get_last_turn_lenght ();
	
	owner . get_movable_model () . move (0, turn_lenght);
	owner . get_movable_model () . turn (0, turn_lenght);
	
	if (! owner . hands . is_empty ())
	{
		return Fight_State :: get ();
	}

	return owner . get_active_state ();
}
