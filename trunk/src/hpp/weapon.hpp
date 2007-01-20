#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "entity.hpp"

using namespace std;

namespace tsl
{
	class Weapon :
	public Entity
	{
		public :
			Weapon
				(float new_volume,
				float new_weight,
				btVector3 new_position,
				float new_sharpness,
				float new_break_chance,
				float new_speed,
				float new_attack_rate,
				float new_defense_rate,
				float new_damage,
				Ogre :: SceneNode & new_node);
			virtual ~Weapon ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			bool broken;
			float sharpness;	//	in percent
			const float break_chance;	//	in percent, 0.01 % is realistic
			const float speed;	//	weapon speed with character's speed 10
			const float attack_rate;	//	the average attack with skill 10
			const float defense_rate;	//	the average defense with skill 10
			const float damage;	//	the average dmg when fully sharp
	};
}

#endif	//	WEAPON_HPP
