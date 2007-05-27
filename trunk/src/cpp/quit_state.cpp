#include "log.hpp"
#include "quit_state.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Algorithms;

//	static
const string Quit_State ::
	get_class_name ()
{
	return "Quit_State";
}

//  constructor
Quit_State ::
	Quit_State () :
	Object ("quit state")
{
	Engines :: Log :: trace <Quit_State> (me);
	
	//	Do nothing.

	assert (Quit_State :: is_initialized ());
}

//  destructor
Quit_State ::
	~Quit_State ()
{
	Engines :: Log :: trace <Quit_State> (me, "~");
	assert (Quit_State :: is_initialized ());

	//	Do nothing.
}

//	virtual
bool Quit_State ::
	is_initialized ()
	const
{
	return Algorithm <Engines :: Game> :: is_initialized ();
}

//	virtual
Algorithm <Engines :: Game> & Quit_State ::
	transit (Engines :: Game & owner, const Object & message)
{
	assert (is_initialized ());

	//	Return myself.
	return * this;
}
