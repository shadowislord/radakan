#include "alive_state.hpp"
#include "battle_message.hpp"
#include "fight_state.hpp"
#include "log.hpp"
#include "npc.hpp"
#include "movable_model.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Algorithms;

//	static
const string Fight_State ::
	get_class_name ()
{
	return "Fight_State";
}

//  constructor
Fight_State ::
	Fight_State (Alive_State & new_alive_state) :
	Object (new_alive_state . npc . my + "fight state"),
	alive_state (new_alive_state),
	targets (my + "targets")
{
	Engines :: Log :: show (alive_state . npc + " becomes agressive!");

	alive_state . calm = 0;
	
	assert (Fight_State :: is_initialized ());
}

//  destructor
Fight_State ::
	~Fight_State ()
{
	Engines :: Log :: trace (me, Fight_State :: get_class_name (), "~");
	assert (Fight_State :: is_initialized ());

	forget_dependencies ();
}

//	virtual
bool Fight_State ::
	is_initialized ()
	const
{
	assert (Algorithm :: is_initialized ());
	
	return true;
}

//	virtual
Algorithm * Fight_State ::
	transit (const Object & message)
{
	assert (is_initialized ());

	Movable_Model & npc_model = alive_state . npc . get_movable_model ();

	if (message . is_type <Messages :: Battle_Message> ())
	{
		targets . add (message . to_type <Messages :: Battle_Message> () .from);
	}

	if (targets . is_empty ())
	{
		return NULL;
	}

	Items :: Character & target = * targets . get_child ();

	Ogre :: Vector3 target_direction
		= (target . get_model () . node . getPosition () - npc_model . node . getPosition ())
			. normalisedCopy ();

	float flee_modifier = 1.0;	// Don't flee.

	if (alive_state . npc . hands . is_empty ())
	{
		flee_modifier = - 1.0;	//	Flee.
	}

	Ogre :: Vector3 front = npc_model . get_front_direction ();

	//	between - 1 and 1
	float looking_to_goal = flee_modifier * target_direction . dotProduct (front);
	
	//	The more I look to my goal, the less I'll turn.
	npc_model . turn
	(
		Ogre :: Math :: Sqrt (0.5 - looking_to_goal / 2.0),
		- flee_modifier * target_direction . crossProduct (front)
	);

	if
	(
		(flee_modifier == 1)
			&& (
				1 < npc_model . node . getPosition () . distance
					(target . get_model () . node . getPosition ())
			)
	)
	{
		npc_model . move (0.5 + looking_to_goal / 2.0);
	}
	else
	{
		npc_model . move (0);
	}

	return this;
}
