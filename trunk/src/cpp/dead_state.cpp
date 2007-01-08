#include "dead_state.hpp"

using namespace std;
using namespace tsl;

//  constructor
Dead_State ::
	Dead_State (Character & new_owner) :
	Object (new_owner + "'s dead state"),
	State (new_owner)
{
	assert (State :: is_initialized ());

	assert (Dead_State :: is_initialized ());
}

//  destructor
Dead_State ::
	~Dead_State ()
{
	trace () << "~Dead_State ()" << endl;
	assert (State :: is_initialized ());
}

//	virtual
bool Dead_State ::
	is_initialized ()
	const
{
	return State :: is_initialized ();
}

//	static
string Dead_State ::
	get_type_name ()
{
	return "dead state";
}

//	virtual
string Dead_State ::
	think ()
{
	assert (is_initialized ());

	error () << "- The deads can't think." << endl;
	abort ();

	return "The deads can't think.";
}
