#include "weapon.hpp"

using namespace std;
using namespace TSL;

//	static
const string Weapon ::
	class_name ("Weapon");

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
	assert (Item :: is_initialized ());

	broken = false;
	sharpness = new_sharpness;

	assert (is_initialized ());
}

//  destructor
Weapon ::
	~Weapon ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
	assert (is_initialized ());
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

//	static
Item & Weapon ::
	create
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
	)
{
	Item * temp =
		new Weapon
		(
			new_name,
			new_mesh_name,
			new_size,
			new_mass,
			new_sharpness,
			new_break_chance,
			new_speed,
			new_attack_rate,
			new_defense_rate,
			new_damage
		);

	return * temp;
}
