#include "weapon.hpp"

using namespace std;
using namespace Ogre;

//  Constructor
Weapon::
	Weapon
		(string new_name,
		Vector3 new_size,
		float new_volume,
		float new_weight,
		float new_sharpness,
		float new_break_chance,
		float new_speed,
		float new_attack_rate,
		float new_defense_rate,
		float new_damage):
	Obstacle (new_name, new_size, true, new_volume, new_weight),
	break_chance (new_break_chance),
	speed (new_speed),
	attack_rate (new_attack_rate),
	defense_rate (new_defense_rate),
	damage (new_damage)
{
	assert (Obstacle::is_initialized ());

	broken = false;
	sharpness = new_sharpness;

	assert (is_initialized ());
}

//  Destructor
Weapon::
	~Weapon ()
{
	assert (is_initialized ());
}

//	virtual
bool
	Weapon::
	is_initialized
	()
	const
{
	return Obstacle::is_initialized () && (0 < break_chance)
	&& (break_chance < 100)  && (0 < sharpness) && (sharpness < 100)
	&& (0 < speed) && (0 < attack_rate) && (0 < defense_rate) && (0 < damage);
}
