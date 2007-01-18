#include "state.hpp"

using namespace std;
using namespace tsl;

template <class T> string State <T> :: nothing = "nothing";

//  constructor
template <class T> State <T> ::
	State (T & new_owner) :
	Object (new_owner + "'s generic state"),
	owner (new_owner)
{
	trace () << "State <" << T :: get_type_name () << "> (" << new_owner << ")" << endl;
	assert (Object :: is_initialized ());
	
	assert (State <T> :: is_initialized ());
}

//  destructor
template <class T> State <T> ::
	~State ()
{
	trace () << "~State ()" << endl;
	assert (State <T> :: is_initialized ());
}

//	virtual
template <class T> bool State <T> ::
	is_initialized ()
	const
{
	//	The owner's initalization might depend on this, so do not check that.
	return warn <State <T> > (Object :: is_initialized ());
}

//	static
template <class T> string State <T> ::
	get_type_name ()
{
	return "state";
}

#include "tsl.hpp"

template class State <NPC>;
template class State <TSL>;
