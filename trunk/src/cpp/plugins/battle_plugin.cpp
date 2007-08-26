/*#include "plugins/battle_plugin.hpp"
#include "log.hpp"

using namespace std;
using namespace boost;
using namespace Radakan;
using namespace Radakan :: Plugins;

//	static
const string Battle_Plugin ::
	get_class_name ()
{
	return "Battle_Plugin";
}

Battle_Plugin ::
	Battle_Plugin () :
	Object ("battle state"),
	max_distance (1),
	generator (42u),
	uniform_real_distribution (0, 1),
	lognormal_real_distribution (1.133, 0.5),
	uniform (generator, uniform_real_distribution),
	lognormal (generator, lognormal_real_distribution)
{
	Engines :: Log :: trace (me, Battle_Plugin :: get_class_name ());

	generator . seed (static_cast <unsigned int> (time (0)));

	assert (is_initialized ());
}

Battle_Plugin ::
	~Battle_Plugin ()
{
	Engines :: Log :: trace (me, Battle_Plugin :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Battle_Plugin ::
	is_initialized ()
	const
{
//	assert (Singleton <Battle_Plugin> :: is_initialized ());
	assert (Observer <Strategies :: Play_State> :: is_initialized ());
	
	return true;
}

//	virtual
void Battle_Plugin ::
	call (const Object & message)
{
	assert (is_initialized ());
	if (message . name != "battle")
	{
		return;
	}

	//	TODO
}

void Battle_Plugin ::
	add (Character & attacker, Character & defender)
{
	Engines :: Log :: trace (me, Battle_Plugin :: get_class_name (), "add", attacker, defender);
	assert (is_initialized ());

	assert (! attacker . is_dead ());

	if (defender . is_dead ())
	{
		Engines :: Log :: show ("Mutilating " + defender . name + "'s dead body is *not* nice.");
		return;
	}

	float distance =
		(
			attacker . get_model () . node . getPosition ()
			- defender . get_model () . node . getPosition ()
		) . length ();
	
	if (max_distance < distance)
	{
		Engines :: Log :: show ("Target is out of range: "
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

	Engines :: Log :: log (me) << "Atack: " << attack << endl;
	Engines :: Log :: log (me) << "Defense: " << defense << endl;

	if (defense < attack)	//	Hit
	{
		defender . die ();
	}
	else
	{
		Engines :: Log :: show (attacker . name + " missed!");
	}
}*/
