#include "log.hpp"
#include "quit_state.hpp"

using namespace std;
using namespace TSL;

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
	Log :: trace <Quit_State> (me);
	assert (Algorithm <Game> :: is_initialized ());

	assert (Quit_State :: is_initialized ());
}

//  destructor
Quit_State ::
	~Quit_State ()
{
	Log :: trace <Quit_State> (me, "~");

	assert (Algorithm <Game> :: is_initialized ());
}

//	virtual
bool Quit_State ::
	is_initialized ()
	const
{
	return Algorithm <Game> :: is_initialized ();
}
