#include "battle_engine.hpp"

using namespace std;
using namespace boost;

Battle_Engine ::
	Battle_Engine () :
	Engine ("Battle engine"),
	generator (42u),
	uniform_real_distribution (0, 1),
	lognormal_real_distribution (1.133, 0.5),
	uniform (generator, uniform_real_distribution),
	lognormal (generator, lognormal_real_distribution)
{
	assert (Object :: is_initialized ());

	generator.seed (static_cast <unsigned int> (time (0)));

	assert (is_initialized ());
}

Battle_Engine ::
	~Battle_Engine ()
{
	assert (is_initialized ());
}

//	virtual
bool Battle_Engine ::
	is_initialized ()
	const
{
	return Engine :: is_initialized ();
}

string Battle_Engine ::
	hit (Character * attacker, Character * defender)
{
	assert (is_initialized ());
	assert (attacker != NULL);
	assert (defender != NULL);

	assert (! attacker -> is_dead ());
	assert (! defender -> is_dead ());
	
	if (1 < (attacker -> node -> getPosition () - defender -> node -> getPosition ()) . squaredLength ())
	{
		return "Target is out of range.";
	}

	float attack = 0;
	float defense = 0;
	
	if (attacker -> has_weapon ())
	{
		attack = attacker -> get_weapon () -> attack_rate * lognormal ();
	}

	if (defender -> has_weapon ())
	{
		defense = defender -> get_weapon () -> defense_rate * lognormal ();
	}

	// You can now retrieve random numbers from that distribution by means
	// of a STL Generator interface, i.e. calling the generator as a zero-
	// argument function.
//		for (int i = 0; i < 3; i++)
//		{
//			debug () << "Uniform (0, 1): " << uniform () << endl;
//		}
//		for (int i = 0; i < 5; i++)
//		{
//			debug () << "Log-normal (1.133, 0.5): " << lognormal () << endl;
//		}

	debug () << "Atack: " << to_string (attack) << endl;
	debug () << "Defend: " << to_string (defense) << endl;

	if (defense < attack)	//	Hit
	{
		return defender -> die ();
	}
	else
	{
		return * attacker + " missed!";
	}
}
