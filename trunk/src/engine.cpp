#include "engine.hpp"

using namespace std;
using namespace boost;

Engine ::
	Engine (string new_name) :
	Object (new_name)
{
	assert (Engine :: is_initialized ());

	assert (is_initialized ());
}

Engine ::
	~Engine ()
{
	assert (is_initialized ());
}

//	virtual
bool Engine ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

Event * Battle_Engine ::
	process (Event * event)
{
	abort ();

	return NULL;
}
