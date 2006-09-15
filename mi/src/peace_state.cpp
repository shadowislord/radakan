#include "peace_state.hpp"
#include "fight_state.hpp"

using namespace std;

//  constructor
Peace_State ::
	Peace_State (Character * new_owner, State * new_parent_state):
	Object (* new_owner + "'s peace state"),
	State (new_owner, new_parent_state)
{
	assert (Peace_State :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
Peace_State ::
	~Peace_State ()
{
	assert (Object :: is_initialized (* this + "->~Peace_State ()"));
}

//	virtual
bool Peace_State ::
	is_initialized ()
	const
{
	return State :: is_initialized ();
}

//	virtual
void Peace_State ::
	act ()
{
	debug () << "Hey!" << endl;
}

//	virtual
void Peace_State ::
	think (State * parent)
{
	debug () << "nothing to worry..." << endl;
}
