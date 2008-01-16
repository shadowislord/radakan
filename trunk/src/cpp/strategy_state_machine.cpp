#include "engines/log.hpp"
#include "strategy_state_machine.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T, class U> string Strategy_State_Machine <T, U> ::
	get_class_name ()
{
	return "Strategy_State_Machine <" + T :: get_class_name ()
		+ ", " + U :: get_class_name () + ">";
}

//  constructor
template <class T, class U> Strategy_State_Machine <T, U> ::
	Strategy_State_Machine () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Engines :: Log :: trace
		(this -> me, Strategy_State_Machine <T, U> :: get_class_name (), "");
	
	//	Do nothing.

	assert (Strategy_State_Machine :: is_initialized ());
}

//  destructor
template <class T, class U> Strategy_State_Machine <T, U> ::
	~Strategy_State_Machine ()
{
	Engines :: Log :: trace
		(this -> me, Strategy_State_Machine <T, U> :: get_class_name (), "~");
	assert (Strategy_State_Machine :: is_initialized ());

	//	Do nothing.
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
}

//	virtual
template <class T, class U> bool Strategy_State_Machine <T, U> ::
	is_initialized ()
	const
{
	assert (State_Machine <T> :: is_initialized ());

	return true;
}

template <class T, class U> void Strategy_State_Machine <T, U> ::
	run (const Reference <Messages :: Message <U> > & message)
{
	assert (is_initialized ());

	if (this -> has_active_state ())
	{
		Reference <T> old_state = this -> get_active_state ();
		Reference <T> new_state = old_state -> transit (message);

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

#include "items/character.hpp"
#include "strategies/actions/action.hpp"
#include "strategies/behaviors/behavior.hpp"
#include "strategies/game_modes/game_mode.hpp"

template class Strategy_State_Machine <Strategies :: Actions :: Action, Items :: Character>;
template class Strategy_State_Machine
	<Strategies :: Behaviors :: Behavior, Items :: Character>;
template class Strategy_State_Machine <Strategies :: Game_Modes :: Game_Mode, Object>;
