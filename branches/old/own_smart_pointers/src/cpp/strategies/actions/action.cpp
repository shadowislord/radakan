#include "engines/log.hpp"
#include "items/character.hpp"
#include "strategies/actions/action.hpp"
#include "strategies/behaviors/ai.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Actions;

//	static
string Action ::
	get_class_name ()
{
	return "Strategies :: Actions :: Action";
}

//  constructor
Action ::
	Action
	(
		Reference <Items :: Character> new_character,
		Reference <Behaviors :: AI> new_ai
	) :
	Object ("The name doesn't matter. This is an abstract base class."),
	character (new_character),
	ai (new_ai)
{
	//	Do nothing.

	assert (is_initialized ());
}

//  destructor
Action ::
	~Action ()
{
	Engines :: Log :: trace (me, Action :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Action ::
	is_initialized ()
	const
{
	//	'assert' can't handle double templates.
	//	assert (Strategy <Action, Messages :: Communications :: Communication> :: is_initialized ());
	
	return true;
}
