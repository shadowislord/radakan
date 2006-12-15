#include "battle_engine.hpp"

using namespace std;
using namespace boost;
using namespace sl;

template <> Battle_Engine * Ogre :: Singleton <Battle_Engine> :: ms_Singleton = NULL;

Battle_Engine ::
	Battle_Engine () :
	Object ("Battle engine"),
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
	trace () << "~Battle_Engine ()" << endl;
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
	get_type_name ()
{
	return "battle engine";
}

string Battle_Engine ::
	hit (Character & attacker, Character & defender)
{
	trace () << "hit (" << attacker << ", " << defender << ")" << endl;
	assert (is_initialized ());

	assert (! attacker . is_dead ());
	assert (! defender . is_dead ());
	
	if (100 < (attacker . node -> getPosition () - defender . node -> getPosition ()) . squaredLength ())
	{
		return "Target is out of range: " + to_string (((attacker . node -> getPosition () - defender . node -> getPosition ()) . squaredLength ()) - 100);
	}

	float attack = 0;
	float defense = 0;
	
	if (attacker . has_weapon ())
	{
		attack = attacker . get_weapon () -> attack_rate * lognormal ();
	}

	if (defender . has_weapon ())
	{
		defense = defender . get_weapon () -> defense_rate * lognormal ();
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

	debug () << "Atack: " << attack << endl;
	debug () << "Defend: " << defense << endl;

	if (defense < attack)	//	Hit
	{
		return defender . die ();
	}
	else
	{
		return attacker + " missed!";
	}
}
