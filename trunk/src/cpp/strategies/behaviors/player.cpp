#include "engines/log.hpp"
#include "items/characters/character.hpp"
#include "strategies/behaviors/player.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Behaviors;

//	static
string Player ::
	get_class_name ()
{
	return "Player";
}

//  constructor
Player ::
	Player (Reference <Items :: Characters :: Character> new_character) :
	Object (new_character . get_name () + "'s player"),
	Behavior (new_character)
{
	//	Do nothing.

	assert (Player :: is_initialized ());
}

//  destructor
Player ::
	~Player ()
{
	Engines :: Log :: trace (me, Player :: get_class_name (), "~");
	assert (Player :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Player ::
	is_initialized ()
	const
{
	//	'assert' can't handle double templates.
	//	assert (Strategy <Behavior, Items :: Characters :: Character> :: is_initialized ());

	return true;
}

//	virtual
Reference <Behavior> Player ::
	transit
		(const Reference <Messages :: Message <Items :: Characters :: Character> > & message)
{
	assert (is_initialized ());
	
	return Reference <Behavior> (this);
}
