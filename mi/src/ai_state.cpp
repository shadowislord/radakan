#include "ai_state.hpp"

using namespace std;

//  constructor
AI_State ::
	AI_State (string new_name):
	Object (new_name)
{
	assert (Object :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
AI_State ::
	~AI_State ()
{
	assert (Object :: is_initialized (* this + "->~AI_State ()"));
}

//	virtual
bool AI_State ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}
