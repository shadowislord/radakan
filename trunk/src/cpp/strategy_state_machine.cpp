#include "strategy_state_machine.hpp"
#include "strategy.hpp"
#include "log.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;

//	static
string Strategy_State_Machine ::
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

void Strategy_State_Machine ::
	run (const Reference <Object> message)
{
	assert (is_initialized ());

	if (has_active_state ())
	{
		Reference <Strategy> old_state = get_active_state ();
		Reference <Strategy> new_state = old_state -> transit (message);
		if (new_state . points_to_object ())
		{
			set_active_state (new_state);
		}
		else
		{
			drop (old_state);
		}
	}
}
