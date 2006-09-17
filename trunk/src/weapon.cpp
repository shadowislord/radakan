#include "weapon.hpp"

using namespace std;

//  constructor
Weapon::
	Weapon
		(float new_volume,
		float new_weight,
		Ogre :: Vector3 new_position,
		float new_sharpness,
		float new_break_chance,
		float new_speed,
		float new_attack_rate,
		float new_defense_rate,
		float new_damage,
		Ogre :: Entity * new_ogre_entity,
		Ogre :: SceneNode * new_node) :
	Object
		((new_ogre_entity == NULL) ?
		"[ERROR: new_ogre_entity is NULL]" :
		new_ogre_entity->getName ()),
	Entity
		(true,
		true,
		true,
		new_volume,
		new_weight,
		new_position,
		new_ogre_entity,
		new_node),
	break_chance (new_break_chance),
	speed (new_speed),
	attack_rate (new_attack_rate),
	defense_rate (new_defense_rate),
	damage (new_damage)
{
	assert (Entity :: is_initialized ());

	broken = false;
	sharpness = new_sharpness;

	assert (is_initialized ());
}

//  destructor
Weapon::
	~Weapon ()
{
	assert (is_initialized ());
}

//	virtual
bool Weapon ::
	is_initialized ()
	const
{
	return Entity :: is_initialized () && (0 < break_chance)
	&& (break_chance < 100)  && (0 < sharpness) && (sharpness < 100)
	&& (0 < speed) && (0 < attack_rate) && (0 < defense_rate) && (0 < damage);
}
