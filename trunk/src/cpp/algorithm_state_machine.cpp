#include "algorithm_state_machine.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <class T> Algorithm_State_Machine <T> ::
	Algorithm_State_Machine () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Object :: trace () << get_class_name () << " ()" << endl;
	assert (State_Machine <Algorithm <T> > :: is_initialized ());

	assert (Algorithm_State_Machine <T> :: is_initialized ());
}

//  destructor
template <class T> Algorithm_State_Machine <T> ::
	~Algorithm_State_Machine ()
{
	assert (Algorithm_State_Machine <T> :: is_initialized ());

	State_Machine <Algorithm <T> > :: unset_active_state ();
}

//	virtual
template <class T> bool Algorithm_State_Machine <T> ::
	is_initialized ()
	const
{
	assert (Object :: warn <Algorithm_State_Machine <T> > (State_Machine <Algorithm <T> > :: is_initialized ()));

	return true;
}

//	static
template <class T> string Algorithm_State_Machine <T> ::
	get_class_name ()
{
	return "Algorithm_State_Machine <" + T :: get_class_name () + ">";
}

//	virtual
template <class T> void Algorithm_State_Machine <T> ::
	run ()
{
	assert (is_initialized ());
	assert (State_Machine <Algorithm <T> > :: has_active_state ());

	State_Machine <Algorithm <T> > :: set_active_state
	(
		State_Machine <Algorithm <T> > :: get_active_state () . transit
													(Object :: to_type <T> ())
	);
}

//virtual
template <class T> template <typename U> void Algorithm_State_Machine <T> ::
	set_active_state ()
{
	Object :: trace () << "set_active_state <" << U :: get_class_name () << "> ()" << endl;
	assert (Algorithm_State_Machine <T> :: is_initialized ());
	assert (U :: is_instantiated ());
	
	State_Machine <Algorithm <T> > :: set_active_state (U :: get ());
}

//	to avert linking errors:
#include "tsl.hpp"
#include "dead_state.hpp"
#include "peace_state.hpp"
#include "play_state.hpp"
#include "menu_state.hpp"
#include "quit_state.hpp"
#include "fight_state.hpp"

template class Algorithm_State_Machine <NPC>;
template class Algorithm_State_Machine <TSL>;

template void Algorithm_State_Machine <NPC> :: set_active_state <Dead_State> ();
template void Algorithm_State_Machine <NPC> :: set_active_state <Fight_State> ();
template void Algorithm_State_Machine <TSL> :: set_active_state <Menu_State> ();
template void Algorithm_State_Machine <NPC> :: set_active_state <Peace_State> ();
template void Algorithm_State_Machine <TSL> :: set_active_state <Play_State> ();
template void Algorithm_State_Machine <TSL> :: set_active_state <Quit_State> ();
