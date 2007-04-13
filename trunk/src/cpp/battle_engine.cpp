#include "battle_engine.hpp"

using namespace std;
using namespace boost;
using namespace tsl;

Battle_Engine ::
	Battle_Engine () :
	Object ("Battle engine"),
	max_distance (1),
	generator (42u),
	uniform_real_distribution (0, 1),
	lognormal_real_distribution (1.133, 0.5),
	uniform (generator, uniform_real_distribution),
	lognormal (generator, lognormal_real_distribution)
{
	log (debugging) << "Battle_Engine ()" << endl;
	assert (Object :: is_initialized ());

	generator . seed (static_cast <unsigned int> (time (0)));

	assert (is_initialized ());
}

Battle_Engine ::
	~Battle_Engine ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;
	assert (is_initialized ());
}

//	virtual
bool Battle_Engine ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	static
string Battle_Engine ::
	get_class_name ()
{
	return "Battle_Engine";
}

void Battle_Engine ::
	hit (Character & attacker, Character & defender)
{
	log (debugging) << "hit (" << attacker << ", " << defender << ")" << endl;
	assert (is_initialized ());

	assert (! attacker . is_dead ());
	assert (! defender . is_dead ());

	float distance =
		(
			attacker . get_body () . node . getPosition ()
			- defender . get_body () . node . getPosition ()
		) . length ();
	
	if (max_distance < distance)
	{
		show () << "Target is out of range: " << to_string (distance) << " > "
													<< to_string (max_distance);
		return;
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

	log (debugging) << "Atack: " << attack << endl;
	log (debugging) << "Defense: " << defense << endl;

	if (defense < attack)	//	Hit
	{
		
		defender . get_body () . turn (- Ogre :: Math :: HALF_PI, 1, defender . get_body () . get_side_direction ());

		defender . die ();
	}
	else
	{
		show () << attacker << " missed!";
	}
}
