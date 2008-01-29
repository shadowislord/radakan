#include "engines/log.hpp"
#include "strategies/strategy.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;

//	static
template <class T, class U> string Strategy <T, U> ::
	get_class_name ()
{
	return "Strategy <" + T :: get_class_name () + ", " + U :: get_class_name () + ">";
}

//  constructor
template <class T, class U> Strategy <T, U> ::
	Strategy () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Engines :: Log :: trace (this -> me, Strategy <T, U> :: get_class_name (), "");
	assert (Strategy :: is_initialized ());

	//	Do nothing.
}

//  destructor
template <class T, class U> Strategy <T, U> ::
	~Strategy ()
{
	Engines :: Log :: trace (this -> me, Strategy <T, U> :: get_class_name (), "~");
	assert (Strategy :: is_initialized ());

	//	Do nothing.
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
}

//	virtual
template <class T, class U> bool Strategy <T, U> ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}

#include "messages/communications/communication.hpp"
#include "messages/nothing.hpp"
#include "strategies/actions/action.hpp"
#include "strategies/behaviors/behavior.hpp"
#include "strategies/game_modes/game_mode.hpp"

template class Strategy
	<Strategies :: Actions :: Action, Messages :: Communications :: Communication>;
template class Strategy <Strategies :: Behaviors :: Behavior, Messages :: Nothing>;
template class Strategy <Strategies :: Game_Modes :: Game_Mode, Messages :: Nothing>;
