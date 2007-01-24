#include "battle_engine.hpp"

using namespace std;
using namespace boost;
using namespace tsl;

Battle_Engine ::
	Battle_Engine () :
	Object ("Battle engine"),
	max_distance (25),
	generator (42u),
	uniform_real_distribution (0, 1),
	lognormal_real_distribution (1.133, 0.5),
	uniform (generator, uniform_real_distribution),
	lognormal (generator, lognormal_real_distribution)
{
	trace () << "Battle_Engine ()" << endl;
	assert (Object :: is_initialized ());

	generator . seed (static_cast <unsigned int> (time (0)));

	assert (is_initialized ());
}

Battle_Engine ::
	~Battle_Engine ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (is_initialized ());
}

//	virtual
bool Battle_Engine ::
	is_initialized ()
	const
{
	return warn <Battle_Engine> (Object :: is_initialized ());
}

//	static
string Battle_Engine ::
	get_class_name ()
{
	return "Battle_Engine";
}

string Battle_Engine ::
	hit (Character & attacker, Character & defender)
{
	trace () << "hit (" << attacker << ", " << defender << ")" << endl;
	assert (is_initialized ());

	assert (! attacker . is_dead ());
	assert (! defender . is_dead ());

	float distance =
		(
			attacker . get_representation () . node . getPosition ()
			- defender . get_representation () . node . getPosition ()
		) . length ();
		
	if (max_distance < distance)
	{
		return "Target is out of range: " + to_string (distance) + " > "
													+ to_string (max_distance);
	}

	float attack = 0;
	float defense = 0;
	
	if (! attacker . hands . is_empty ())
	{
		Item & weapon = * attacker . hands . get_child ();
		if (weapon . is_type <Weapon> ())
		{
			attack = weapon . to_type <Weapon> () . attack_rate * lognormal ();
		}
	}

	if (! defender . hands . is_empty ())
	{
		Item & weapon = * defender . hands . get_child ();
		if (weapon . is_type <Weapon> ())
		{
			defense = weapon . to_type <Weapon> () . defense_rate * lognormal ();
		}
	}

	trace () << "Atack: " << attack << endl;
	trace () << "Defense: " << defense << endl;

	if (defense < attack)	//	Hit
	{
		return defender . die ();
	}
	else
	{
		return attacker + " missed!";
	}
}
