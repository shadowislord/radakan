#include <OgreSceneNode.h>

#include "engines/log.hpp"
#include "items/npc.hpp"
#include "messages/battle_message.hpp"
#include "movable_model.hpp"
#include "set.hpp"
#include "strategies/alive_state.hpp"
#include "strategies/fight_state.hpp"

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
	Fight_State (Reference <Items :: Character> new_character, Reference <Alive_State> new_alive_state) :
	Object (new_character . get_name () + "'s fight state"),
	character (new_character),
	alive_state (new_alive_state),
	targets (new Set <Items :: Character> (me . get_name () + "'s targets"))
{
	Engines :: Log :: show (character . get_name (true) + " becomes agressive!");

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
	assert (Strategy <Items :: Character> :: is_initialized ());
	
	return true;
}

//	virtual
Reference <Strategy <Items :: Character> > Fight_State ::
	transit (const Reference <Messages :: Message <Items :: Character> > & message)
{
	assert (is_initialized ());

	Reference <Movable_Model> npc_model (character -> get_movable_model ());

	if (message . is_castable <Messages :: Battle_Message> ())
	{
		//	The following line doesn't have to return true.
		targets -> add (const_cast <Reference <Items :: Character> &> (message -> from));
	}

	if (targets -> is_empty ())
	{
		return Reference <Strategy <Items :: Character> > ();
	}

	Reference <Items :: Character> target = targets -> get_child ();

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

	return Reference <Strategy <Items :: Character> > (this);
}
