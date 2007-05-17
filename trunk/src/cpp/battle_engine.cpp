#include "battle_engine.hpp"
#include "log.hpp"

using namespace std;
using namespace boost;
using namespace TSL;

//	static
const string Battle_Engine ::
	get_class_name ()
{
	return "Battle_Engine";
}

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
	Log :: trace <Battle_Engine> (me);
	assert (Object :: is_initialized ());

	generator . seed (static_cast <unsigned int> (time (0)));

	assert (is_initialized ());
}

Battle_Engine ::
	~Battle_Engine ()
{
	Log :: trace <Battle_Engine> (me, "~");
	assert (is_initialized ());
}

//	virtual
bool Battle_Engine ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

void Battle_Engine ::
	hit (Character & attacker, Character & defender)
{
	Log :: trace <Battle_Engine> (me, "hit", attacker, defender);
	assert (is_initialized ());

	assert (! attacker . is_dead ());

	if (defender . is_dead ())
	{
		Log :: show ("Mutilating " + defender + "'s dead body is *not* nice.");
		return;
	}

	float distance =
		(
			attacker . get_model () . node . getPosition ()
			- defender . get_model () . node . getPosition ()
		) . length ();
	
	if (max_distance < distance)
	{
		Log :: show ("Target is out of range: "
			+ to_string (distance) + " > " + to_string (max_distance));
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

	Log :: log (me) << "Atack: " << attack << endl;
	Log :: log (me) << "Defense: " << defense << endl;

	if (defense < attack)	//	Hit
	{
		defender . die ();
	}
	else
	{
		Log :: show (attacker + " missed!");
	}
}
