#include "state.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <typename T> State <T> ::
	State (T & new_owner) :
	Object (new_owner + "'s generic state"),
	owner (new_owner)
{
	trace () << "State (" << new_owner << ")" << endl;
	assert (Object :: is_initialized ());
//	trace () << "State (" << new_owner << ") A" << endl;
	
	assert (owner . T :: is_initialized ());
//	trace () << "State (" << new_owner << ") B" << endl;

	assert (State <T> :: is_initialized ());
//	trace () << "State (" << new_owner << ") D" << endl;
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
	return warn <State <T> > (Object :: is_initialized () && owner . T :: is_initialized ());
}

//	static
template <typename T> string State <T> ::
	get_type_name ()
{
	return "state";
}

#include "character.hpp"

template class State <Character>;
