#include "exit_event.hpp"

using namespace std;

//  constructor
Exit_Event ::
	Exit_Event () :
	Object ("Exit Event"),
	Event ("Exit Event")
{
	assert (Event :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
Exit_Event ::
	~Exit_Event ()
{
	assert (Object :: is_initialized (* this + " -> ~Exit_Event ()"));
}

//	virtual
bool Exit_Event ::
	is_initialized ()
	const
{
	return Event :: is_initialized ();

}
