#include "engines/log.hpp"
#include "engines/mediator.hpp"
#include "items/character.hpp"
#include "strategies/behaviors/behavior.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Behaviors;

//	static
string Behavior ::
	get_class_name ()
{
	return "Strategies :: Behaviors :: Behavior";
}

//  constructor
Behavior ::
	Behavior (Reference <Items :: Character> new_character) :
	Object ("The name doesn't matter. This is an abstract base class."),
	character (new_character, true)
{
	add_automatic_destruction_prevention ("construction of " + get_class_name ());
	{
		Reference <Observer <Messages :: Communications :: Communication> > this_behavior
			(this);

		Engines :: Mediator :: get ()
			-> register_observer <Messages :: Communications :: Communication>
				(this_behavior);
	}
	remove_automatic_destruction_prevention ("construction of " + get_class_name ());

	assert (is_initialized ());
}

//  destructor
Behavior ::
	~Behavior ()
{
	Engines :: Log :: trace (me, Behavior :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Behavior ::
	is_initialized ()
	const
{
	//	'assert' can't handle double templates.
	//	assert (Strategy <Behavior, Messages :: Nothing> :: is_initialized ());
	
	return true;
}
