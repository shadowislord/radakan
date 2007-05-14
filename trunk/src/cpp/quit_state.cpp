#include "quit_state.hpp"

using namespace std;
using namespace TSL;

//	static
const string Quit_State ::
	class_name ("Quit_State");


//  constructor
Quit_State ::
	Quit_State () :
	Object ("quit state")
{
	log (debugging) << class_name << " ()" << endl;
	assert (Algorithm <Game> :: is_initialized ());

	assert (Quit_State :: is_initialized ());
}

//  destructor
Quit_State ::
	~Quit_State ()
{
	log (debugging) << "~" << class_name << " ()" << endl;

	assert (Algorithm <Game> :: is_initialized ());
}

//	virtual
bool Quit_State ::
	is_initialized ()
	const
{
	return Algorithm <Game> :: is_initialized ();
}
