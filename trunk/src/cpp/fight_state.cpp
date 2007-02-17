#include "npc.hpp"
#include "fight_state.hpp"
#include "peace_state.hpp"

using namespace std;
using namespace tsl;

//  constructor
Fight_State ::
	Fight_State () :
	Object ("fight state")
{
	assert (Algorithm <NPC> :: is_initialized ());

	assert (Fight_State :: is_initialized ());
}

//  destructor
Fight_State ::
	~Fight_State ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (Algorithm <NPC> :: is_initialized ());
}

//	virtual
bool Fight_State ::
	is_initialized ()
	const
{
	return Algorithm <NPC> :: is_initialized ();
}

//	static
string Fight_State ::
	get_class_name ()
{
	return "Fight_State";
}

//	virtual
Algorithm <NPC> & Fight_State ::
	transit (NPC & owner)
{
	assert (is_initialized ());

	if (owner . hands . is_empty ())
	{
		return Peace_State :: get ();
	}

	return * this;
}
