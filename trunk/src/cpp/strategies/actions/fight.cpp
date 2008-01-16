#include <OgreSceneNode.h>

#include "engines/log.hpp"
#include "items/characters/character.hpp"
#include "messages/battle_message.hpp"
#include "movable_model.hpp"
#include "set.hpp"
#include "strategies/actions/fight.hpp"
#include "strategies/behaviors/ai.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Actions;

//	static
string Fight ::
	get_class_name ()
{
	return "Fight";
}

//  constructor
Fight ::
	Fight
	(
		Reference <Items :: Characters :: Character> new_character,
		Reference <Behaviors :: AI> new_ai
	) :
	Object (new_character . get_name () + "'s fight action"),
	Action (new_character, new_ai),
	targets (new Set <Items :: Characters :: Character> (me . get_name () + "'s targets"))
{
	Engines :: Log :: show (character . get_name (true) + " becomes aggressive!");

	ai -> calm = 0;
	
	assert (Fight :: is_initialized ());
}

//  destructor
Fight ::
	~Fight ()
{
	Engines :: Log :: trace (me, Fight :: get_class_name (), "~");
	assert (Fight :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Fight ::
	is_initialized ()
	const
{
	//	'assert' can't handle double templates.
	//	assert (Strategy <Action, Items :: Characters :: Character> :: is_initialized ());
	
	return true;
}

//	virtual
Reference <Action> Fight ::
	transit (const Reference <Messages :: Message <Items :: Characters :: Character> > & message)
{
	assert (is_initialized ());

	if (message -> to == character)
	{
		if (message . is_castable <Messages :: Battle_Message> ())
		{
			//	The following line doesn't have to return true.
			targets -> add
				(const_cast <Reference <Items :: Characters :: Character> &>
					(message -> from));
		}
	}

	if (targets -> is_empty ())
	{
		return Reference <Action> ();
	}

	Reference <Items :: Characters :: Character> target = targets -> get_child ();
	Reference <Movable_Model> npc_model (character -> get_movable_model ());

	Ogre :: Vector3 target_direction
		= (target -> get_model () -> node -> getPosition () - npc_model -> node -> getPosition ())
			. normalisedCopy ();

	float flee_modifier = 1.0;	// Don't flee.

	if (character -> right_hand -> is_empty ())
	{
		flee_modifier = - 1.0;	//	Flee.
	}

	Ogre :: Vector3 front = npc_model -> get_front_direction ();

	//	between - 1 and 1
	float looking_to_goal = flee_modifier * target_direction . dotProduct (front);
	
	//	The more I look to my goal, the less I'll turn.
	npc_model -> turn
	(
		Ogre :: Math :: Sqrt (0.5 - looking_to_goal / 2.0),
		- flee_modifier * target_direction . crossProduct (front)
	);

	if
	(
		(flee_modifier == 1)
			&& (
				1 < npc_model -> node -> getPosition () . distance
					(target -> get_model () -> node -> getPosition ())
			)
	)
	{
		npc_model -> move (0.5 + looking_to_goal / 2.0);
	}
	else
	{
		npc_model -> move (0);
	}

	return Reference <Action> (this);
}

//	virtual
string Fight ::
	get_action_name ()
{
	return get_class_name ();
}
