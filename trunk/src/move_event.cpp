#include "move_event.hpp"

using namespace std;

//  constructor
Move_Event ::
	Move_Event (string direction) :
	Object (direction),
	Event (direction)
{
	assert (Event :: is_initialized ());
	is_direction (direction);

	assert (is_initialized ());
}

//  destructor
Move_Event ::
	~Move_Event ()
{
	assert (Object :: is_initialized (* this + " -> ~Exit_Event ()"));
}

//	virtual
bool Move_Event ::
	is_initialized ()
	const
{
	return Event :: is_initialized () && is_direction (* this);
}

bool Move_Event ::
	is_direction (string direction)
	const
{
	return (direction == forward) || (direction == backward) || (direction == right) || (direction == left);
}

const string Move_Event :: forward = "forward";
const string Move_Event :: backward = "backward";
const string Move_Event :: left = "left";
const string Move_Event :: right = "right";
