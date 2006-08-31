#include "battle_engine.hpp"

using namespace std;
using namespace Ogre;
using namespace boost;

Battle_Engine::
	Battle_Engine ():
	Object ("Battle engine"),
	generator (42u),
	uniform_real_distribution (0, 1),
	lognormal_real_distribution (1.03, 0.25),
	uniform (generator, uniform_real_distribution),
	lognormal (generator, lognormal_real_distribution)
{
	assert (Object::is_initialized ());

	//	the first result is not random
	uniform ();
	lognormal ();

	assert (is_initialized ());
}

Battle_Engine::
	~Battle_Engine ()
{
	assert (is_initialized ());
}

//	virtual
bool
	Battle_Engine::
	is_initialized ()
	const
{
	return Object::is_initialized ();
}

string
	Battle_Engine::
	hit (Character * attacker, Character * defender)
{
	if (1 < (attacker->position - defender->position).squaredLength ())
	{
		return "The target is out of reach.";
	}

	// You can now retrieve random numbers from that distribution by means
	// of a STL Generator interface, i.e. calling the generator as a zero-
	// argument function.
	for (int i = 0; i < 10; i++)
	{
		debug () << "Uniform (0, 1): " << uniform () << endl;
	}
	for (int i = 0; i < 20; i++)
	{
		debug () << "Log-normal (1.03, 0.25): " << lognormal () << endl;
	}

	return "Error";
}
