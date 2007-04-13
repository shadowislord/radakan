#include "quit_state.hpp"

using namespace std;
using namespace tsl;

//  constructor
Quit_State ::
	Quit_State () :
	Object ("quit state")
{
	log (debugging) << get_class_name () << " ()" << endl;
	assert (Algorithm <TSL> :: is_initialized ());

	assert (Quit_State :: is_initialized ());
}

//  destructor
Quit_State ::
	~Quit_State ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;

	assert (Algorithm <TSL> :: is_initialized ());
}

//	virtual
bool Quit_State ::
	is_initialized ()
	const
{
	return Algorithm <TSL> :: is_initialized ();
}

//	static
string Quit_State ::
	get_class_name ()
{
	return "Quit_State";
}
