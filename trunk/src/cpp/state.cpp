#include "state.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <typename T> State <T> ::
	State (T & new_owner) :
	Object (new_owner + "'s generic state"),
	owner (new_owner)
{
	trace () << "State <" << T :: get_type_name () << "> (" << new_owner << ")" << endl;
	assert (Object :: is_initialized ());
	
	assert (State <T> :: is_initialized ());
}

//  destructor
template <typename T> State <T> ::
	~State ()
{
	trace () << "~State ()" << endl;
	assert (State <T> :: is_initialized ());
}

//	virtual
template <typename T> bool State <T> ::
	is_initialized ()
	const
{
	//	The owner's initalization might depend on this, so do not check that.
	return warn <State <T> > (Object :: is_initialized ());
}

//	static
template <typename T> string State <T> ::
	get_type_name ()
{
	return "state";
}

#include "tsl.hpp"

template class State <NPC>;
template class State <TSL>;
