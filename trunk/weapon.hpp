#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "obstacle.hpp"

using namespace std;

class Weapon:
	public Obstacle
{
	public:
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
			float new_damage);
		virtual ~Weapon ();
		virtual bool is_initialized () const;

		bool broken;
		float sharpness;	//	in percent
		const float break_chance;	//	in percent, 0.01 % is realistic
		const float speed;	//	weapon speed with character's speed 10
		const float attack_rate;	//	the average attack with skill 10
		const float defense_rate;	//	the average defense with skill 10
		const float damage;	//	the average dmg when fully sharp
};

#endif	//	WEAPON_HPP
