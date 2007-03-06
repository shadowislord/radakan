#include "npc.hpp"
#include "fight_state.hpp"
#include "peace_state.hpp"

using namespace std;
using namespace tsl;

//  constructor
Peace_State ::
	Peace_State () :
	Object ("peace state")
{
	assert (Algorithm <NPC> :: is_initialized ());

	assert (Peace_State :: is_initialized ());
}

//  destructor
Peace_State ::
	~Peace_State ()
{
	log (TSL_DEBUG) << "~" << get_class_name () << " ()" << endl;
	
	assert (Algorithm <NPC> :: is_initialized ());
}

//	virtual
bool Peace_State ::
	is_initialized ()
	const
{
	assert (Algorithm <NPC> :: is_initialized ());
	
	return true;
}

//	static
string Peace_State ::
	get_class_name ()
{
	return "Peace_State";
}

//	virtual
Algorithm <NPC> & Peace_State ::
	transit (NPC & owner)
{
	assert (is_initialized ());
	
	if (! owner . hands . is_empty ())
	{
		return Fight_State :: get ();
	}

	return * this;
}
