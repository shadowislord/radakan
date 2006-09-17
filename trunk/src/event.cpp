#include "event.hpp"

using namespace std;

//  constructor
Event ::
	Event (string new_name) :
	Object (new_name)
{
	assert (Object :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
Event ::
	~Event ()
{
	assert (Object :: is_initialized (* this + " -> ~Event ()"));
}

//	virtual
bool Event ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}


void Event ::
	set_event (char event)
{
}

char Event ::
	get_event ()
{
	return 0;
}
