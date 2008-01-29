#include <OgreSceneNode.h>

#include "engines/log.hpp"
#include "items/character.hpp"
#include "messages/communications/fight.hpp"
#include "movable_model.hpp"
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
string Fighting ::
	get_action_name ()
{
	return get_class_name ();
}
