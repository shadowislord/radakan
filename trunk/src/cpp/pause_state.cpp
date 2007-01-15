#include "pause_state.hpp"

using namespace std;
using namespace tsl;

//  constructor
Pause_State ::
	Pause_State (TSL & new_owner) :
	Object (new_owner + "'s pause state"),
	State <TSL> (new_owner)
{
	assert (State <TSL> :: is_initialized ());

	assert (Pause_State :: is_initialized ());
}

//  destructor
Pause_State ::
	~Pause_State ()
{
	trace () << "~Pause_State ()" << endl;
	assert (State <TSL> :: is_initialized ());
}

//	virtual
bool Pause_State ::
	is_initialized ()
	const
{
	return State <TSL> :: is_initialized ();
}

//	static
string Pause_State ::
	get_type_name ()
{
	return "pause state";
}

//	virtual
string Pause_State ::
	run ()
{
	assert (is_initialized ());

	return "";
}
