#include "engines/log.hpp"
#include "items/weapon.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Items;

//	static
string Weapon ::
	get_class_name ()
{
	return "Weapon";
}

//  constructor
Weapon ::
	Weapon
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass,
		float new_sharpness,
		float new_break_chance,
		float new_speed,
		float new_attack_rate,
		float new_defense_rate,
		float new_damage
	) :
	Object (new_name),
	Item
	(
		new_mesh_name,
		new_size,
		new_mass,
		true,
		true,
		true
	),
	break_chance (new_break_chance),
	speed (new_speed),
	attack_rate (new_attack_rate),
	defense_rate (new_defense_rate),
	damage (new_damage)
{
	broken = false;
	sharpness = new_sharpness;

	assert (is_initialized ());
}

//  destructor
Weapon ::
	~Weapon ()
{
	Engines :: Log :: trace (me, Weapon :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Weapon ::
	is_initialized ()
	const
{
	return Item :: is_initialized () && (0 < break_chance)
	&& (break_chance < 100)  && (0 < sharpness) && (sharpness < 100)
	&& (0 < speed) && (0 < attack_rate) && (0 < defense_rate) && (0 < damage);
}
