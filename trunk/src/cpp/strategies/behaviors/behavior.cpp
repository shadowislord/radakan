#include "engines/log.hpp"
#include "items/characters/character.hpp"
#include "strategies/behaviors/behavior.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Behaviors;

//	static
string Behavior ::
	get_class_name ()
{
	return "Behavior";
}

//  constructor
Behavior ::
	Behavior (Reference <Items :: Characters :: Character> new_character) :
	Object ("The name doesn't matter. This is an abstract base class."),
	character (new_character)
{
	//	Do nothing.

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
	//	assert (Strategy <Behavior, Items :: Characters :: Character> :: is_initialized ());
	
	return true;
}
