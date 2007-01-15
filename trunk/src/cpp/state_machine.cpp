#include "state_machine.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <typename T> State_Machine <T> ::
	State_Machine (T & new_owner) :
	Object (new_owner + "'s state machine"),
	State <T> (new_owner),
	Set <State <T> > (new_owner + "'s state machine")
{
	Object :: trace () << "State_Machine <" << T :: get_type_name () << "> ()" << endl;
	assert (Set <State <T> > :: is_initialized ());

	active_child_state = NULL;
	
	assert (State_Machine <T> :: is_initialized ());
}

//  destructor
template <typename T> State_Machine <T> ::
	~State_Machine ()
{
	assert (State_Machine :: is_initialized ());
}

//	virtual
template <typename T> bool State_Machine <T> ::
	is_initialized ()
	const
{
	assert (Object :: warn <State_Machine <T> > (State <T> :: is_initialized ()));
	assert (Object :: warn <State_Machine <T> > (Set <State <T> > :: is_initialized ()));

	return (Object :: warn <State_Machine <T> > ((active_child_state == NULL) || active_child_state -> is_initialized ()));
}

//	static
template <typename T> string State_Machine <T> ::
	get_type_name ()
{
	return "state machine <" + T :: get_type_name () + ">";
}

//	virtual
template <typename T> string State_Machine <T> ::
	run ()
{
	assert (is_initialized ());
	assert (active_child_state != NULL);
	
	return active_child_state -> run ();
}

//	virtual
template <typename T> bool State_Machine <T> ::
	add (State <T> & state)
{
	assert (is_initialized ());
	assert (state . is_initialized ());

	bool result = Set <State <T> > :: add (state);

	assert (result);

	if (active_child_state == NULL)
	{
		active_child_state = & state;
	}

	return true;
}

//	virtual
template <typename T> State <T> * State_Machine <T> ::
	get_active_state ()
{
	if (active_child_state == NULL)
	{
		return this;
	}
	if (active_child_state -> Object :: is_type <State_Machine <T> > ())
	{
		return active_child_state
			-> Object :: to_type <State_Machine <T> > ()
			. get_active_state ();
	}
	return active_child_state;
}

//	virtual
template <typename T> void State_Machine <T> ::
	change_active_state (State <T> & state)
{
	Object :: trace () << "change_active_state (" << state << ")" << endl;
	assert (State_Machine <T> :: is_initialized ());
	assert (contains (state, true));

	if (contains (state, false))
	{
		//	Object :: trace () << state << " is my child" << endl;
		active_child_state = & state;
	}
	else
	{
		//	Object :: trace () << "Maybe " << state << " belongs to my children." << endl;
		for (State <T> * i = Set <State <T> > :: get_child (); i != NULL;
								i = Set <State <T> > :: get_another_child ())
		{
			if (i -> Object :: is_type <State_Machine <T> > ())
			{
				//	Object :: trace () << "Maybe " << state << " belongs to " << * i << endl;
				State_Machine <T> & child_machine =
								i -> Object :: to_type <State_Machine <T> > ();
				if (child_machine . contains (state, true))
				{
					//	Object :: trace () << state << " belongs to " << * i << "!" << endl;
					child_machine . change_active_state (state);
					active_child_state = & child_machine;
					
					return;
				}
			}
		}

		Object :: error () << "could not find the parent state." << endl;
		abort ();
	}
	
	assert (active_child_state != NULL);
}

//virtual
template <typename T> template <typename U> void State_Machine <T> ::
	change_active_state ()
{
	Object :: trace () << "change_active_state <" << U :: get_type_name () << "> ()" << endl;
	assert (Object :: is_initialized ());

	//	'template' added to assure that get_typed_child is a template method.
	active_child_state = Set <State <T> > :: template get_typed_child <U> ();

	if (active_child_state == NULL)
	{
		add <U> ();
		//	'template' added to assure that get_typed_child is a template method.
		active_child_state = Set <State <T> > :: template get_typed_child <U> ();
	}

	assert (active_child_state != NULL);
}

template <typename T> template <typename U> void State_Machine <T> ::
	add ()
{
	Object :: trace () << "add <" << U :: get_type_name () << "> ()" << endl;
	assert (is_initialized ());

	//	'template' added to assure that get_typed_child is a template method.
	assert (Set <State <T> > :: template get_typed_child <U> () == NULL);

	add (* (new U (State <T> :: owner)));
}

//	to avert linking errors:
#include "pause_state.hpp"
#include "play_state.hpp"

template class State_Machine <NPC>;
template class State_Machine <TSL>;

template void State_Machine <NPC> :: change_active_state <Dead_State> ();
template void State_Machine <NPC> :: change_active_state <Fight_State> ();
template void State_Machine <NPC> :: change_active_state <Peace_State> ();

template void State_Machine <NPC> :: add <Dead_State> ();
template void State_Machine <NPC> :: add <Fight_State> ();
template void State_Machine <NPC> :: add <Peace_State> ();
template void State_Machine <TSL> :: add <Pause_State> ();
template void State_Machine <TSL> :: add <Play_State> ();
