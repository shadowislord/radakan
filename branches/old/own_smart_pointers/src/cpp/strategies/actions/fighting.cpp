#include <OgreSceneNode.h>

#include "body.hpp"
#include "engines/log.hpp"
#include "items/character.hpp"
#include "messages/communications/fight.hpp"
#include "set.hpp"
#include "strategies/actions/fighting.hpp"
#include "strategies/behaviors/ai.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Actions;

//	static
string Fighting ::
	get_class_name ()
{
	return "Strategies :: Actions :: Fighting";
}

//  constructor
Fighting ::
	Fighting
	(
		Reference <Items :: Character> new_character,
		Reference <Behaviors :: AI> new_ai
	) :
	Object (new_character . get_name () + "'s fight action"),
	Action (new_character, new_ai),
	targets (new Set <Items :: Character> (me . get_name () + "'s targets"))
{
	Engines :: Log :: show (character . get_name (true) + " becomes aggressive!");

	ai -> calm = 0;
	
	assert (Fighting :: is_initialized ());
}

//  destructor
Fighting ::
	~Fighting ()
{
	Engines :: Log :: trace (me, Fighting :: get_class_name (), "~");
	assert (Fighting :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Fighting ::
	is_initialized ()
	const
{
	//	'assert' can't handle double templates.
	//	assert (Strategy <Action, Messages :: Communications :: Communication> :: is_initialized ());
	
	return true;
}

//	virtual
Reference <Action> Fighting ::
	transit (Reference <Messages :: Communications :: Communication> message)
{
	assert (is_initialized ());

	if (message -> to == character)
	{
		if (message . is_castable <Messages :: Communications :: Fight> ())
		{
			//	The following line doesn't have to return true.
			targets -> add
				(const_cast <Reference <Items :: Character> &> (message -> from));
		}
	}

	if (targets -> is_empty ())
	{
		return Reference <Action> ();
	}

	Reference <Items :: Character> target = targets -> get_child ();
	Reference <Body> npc_body (character -> get_body ());

	Mathematics :: Vector_3D target_direction
		= (target -> get_body () -> get_position () - npc_body -> get_position ())
			. normalisedCopy ();

	float flee_modifier = 1.0;	// Don't flee.

	if (character -> right_hand -> is_empty ())
	{
		flee_modifier = - 1.0;	//	Flee.
	}

	const Mathematics :: Vector_3D & front = npc_body -> get_front_direction ();

	//	between - 1 and 1
	float looking_to_goal = flee_modifier * target_direction . dotProduct (front);
	
	//	The more I look to my goal, the less I'll turn.
	npc_body -> turn
	(
		Ogre :: Math :: Sqrt (0.5 - looking_to_goal / 2.0),
		- flee_modifier * target_direction * front
	);

	if
	(
		(flee_modifier == 1)
			&& (
				1 < npc_body -> get_position () . distance
					(target -> get_body () -> get_position ())
			)
	)
	{
		npc_body -> walk (0.5 + looking_to_goal / 2.0);
	}
	else
	{
		npc_body -> walk (0);
	}

	return Reference <Action> (this);
}

//	virtual
string Fighting ::
	get_action_name ()
{
	return get_class_name ();
}
