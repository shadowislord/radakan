#include "state.hpp"

using namespace std;
using namespace sl;

//  constructor
State ::
	State (Character & new_owner) :
	Object (new_owner + "'s generic state"),
	owner (new_owner)
{
	trace () << "State (" << new_owner << ")" << endl;
	assert (Object :: is_initialized ());
//	trace () << "State (" << new_owner << ") A" << endl;
	
	assert (owner . Character :: is_initialized ());
//	trace () << "State (" << new_owner << ") B" << endl;
	assert (! owner . is_dead ());
//	trace () << "State (" << new_owner << ") C" << endl;

	assert (State :: is_initialized ());
//	trace () << "State (" << new_owner << ") D" << endl;
}

//  destructor
State ::
	~State ()
{
	trace () << "~State ()" << endl;
	assert (State :: is_initialized ());
}

//	virtual
bool State ::
	is_initialized ()
	const
{
	return warn <State> (Object :: is_initialized () && owner . Character :: is_initialized ());
}

//	static
string State ::
	get_type_name ()
{
	return "state";
}
