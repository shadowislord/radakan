#include "strategy_state_machine.hpp"
#include "log.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Strategies;

//	static
const string Strategy_State_Machine ::
	get_class_name ()
{
	return "Strategy_State_Machine";
}

//  constructor
Strategy_State_Machine ::
	Strategy_State_Machine () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Engines :: Log :: trace (this -> me, Strategy_State_Machine :: get_class_name (), "");
	
	//	Do nothing.

	assert (Strategy_State_Machine :: is_initialized ());
}

//  destructor
Strategy_State_Machine ::
	~Strategy_State_Machine ()
{
	Engines :: Log :: trace (this -> me, Strategy_State_Machine :: get_class_name (), "~");
	assert (Strategy_State_Machine :: is_initialized ());

	//	Do nothing.
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
}

//	virtual
bool Strategy_State_Machine ::
	is_initialized ()
	const
{
	assert (State_Machine <Strategy> :: is_initialized ());

	return true;
}

//	virtual
void Strategy_State_Machine ::
	drop (Object & t, bool stay)
{
	Engines :: Log :: trace (this -> me, Strategy_State_Machine :: get_class_name (), "drop", t . name, bool_to_string (stay));
	assert (is_initialized ());
	
	State_Machine <Strategy> :: drop (t, stay);
}

void Strategy_State_Machine ::
	run (const Object & message, bool old_state_stay)
{
	assert (is_initialized ());

	if (has_active_state ())
	{
		Strategy & old_state = State_Machine <Strategy> :: get_active_state ();
		Strategy * new_state = old_state . transit (message);
		if (new_state == NULL)
		{
			drop (old_state);
		}
		else
		{
			set_active_state (* new_state, old_state_stay);
		}
	}
}
