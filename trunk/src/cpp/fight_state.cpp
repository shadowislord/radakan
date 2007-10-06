#include <OgreSceneNode.h>

#include "alive_state.hpp"
#include "battle_message.hpp"
#include "fight_state.hpp"
#include "log.hpp"
#include "npc.hpp"
#include "movable_model.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;

//	static
string Fight_State ::
	get_class_name ()
{
	return "Fight_State";
}

//  constructor
Fight_State ::
	Fight_State (Reference <Alive_State> new_alive_state) :
	Object (new_alive_state -> npc -> name + "'s fight state"),
	alive_state (new_alive_state),
	targets (new Set <Items :: Character> (name + "'s targets"))
{
	Engines :: Log :: show (alive_state -> npc -> name + " becomes agressive!");

	alive_state -> calm = 0;
	
	assert (Fight_State :: is_initialized ());
}

//  destructor
Fight_State ::
	~Fight_State ()
{
	Engines :: Log :: trace (me, Fight_State :: get_class_name (), "~");
	assert (Fight_State :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Fight_State ::
	is_initialized ()
	const
{
	assert (Strategy :: is_initialized ());
	
	return true;
}

//	virtual
Reference <Strategy> Fight_State ::
	transit (const Reference <Object> message)
{
	assert (is_initialized ());

	Reference <Movable_Model> npc_model (alive_state -> npc -> get_movable_model ());

	if (message -> is_class <Messages :: Battle_Message> ())
	{
		targets -> add (const_cast <Reference <Items :: Character> &> (message -> to_class_const <Messages :: Battle_Message> () -> from));
	}

	if (targets -> is_empty ())
	{
		return Reference <Strategy> ();
	}

	Reference <Items :: Character> target = targets -> get_child ();

	Ogre :: Vector3 target_direction
		= (target -> get_model () -> node -> getPosition () - npc_model -> node -> getPosition ())
			. normalisedCopy ();

	float flee_modifier = 1.0;	// Don't flee.

	if (alive_state -> npc -> right_hand -> is_empty ())
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

	return Reference <Fight_State> (this);
}
