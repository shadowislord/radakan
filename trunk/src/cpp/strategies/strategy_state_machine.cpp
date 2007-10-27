#include "engines/log.hpp"
#include "strategies/strategy_state_machine.hpp"
#include "strategies/strategy.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;

//	static
template <class T> string Strategy_State_Machine <T> ::
	get_class_name ()
{
	return "Strategy_State_Machine <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Strategy_State_Machine <T> ::
	Strategy_State_Machine () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Engines :: Log :: trace (this -> me, Strategy_State_Machine :: get_class_name (), "");
	
	//	Do nothing.

	assert (Strategy_State_Machine :: is_initialized ());
}

//  destructor
template <class T> Strategy_State_Machine <T> ::
	~Strategy_State_Machine ()
{
	Engines :: Log :: trace (this -> me, Strategy_State_Machine :: get_class_name (), "~");
	assert (Strategy_State_Machine :: is_initialized ());

	//	Do nothing.
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
}

//	virtual
template <class T> bool Strategy_State_Machine <T> ::
	is_initialized ()
	const
{
	assert (State_Machine <Strategy <T> > :: is_initialized ());

	return true;
}

template <class T> void Strategy_State_Machine <T> ::
	run (const Reference <Messages :: Message <T> > & message)
{
	assert (is_initialized ());

	if (this -> has_active_state ())
	{
		Reference <Strategy <T> > old_state = this -> get_active_state ();
		Reference <Strategy <T> > new_state = old_state -> transit (message);
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

#include "engines/game.hpp"
#include "items/character.hpp"

template class Strategy_State_Machine <Engines :: Game>;
template class Strategy_State_Machine <Items :: Character>;
