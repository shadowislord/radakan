#ifndef TSL_WEAPON_HPP
#define TSL_WEAPON_HPP

#include "item.hpp"

using namespace std;

namespace tsl
{
	class Weapon :
	public Item
	{
		public :
			virtual ~Weapon ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			static Item & create
			(
				string new_name,
				string mesh_name,
				float new_volume,
				float new_weight,
				float new_sharpness,
				float new_break_chance,
				float new_speed,
				float new_attack_rate,
				float new_defense_rate,
				float new_damage
			);

			bool broken;
			
			///	in percent
			float sharpness;

			///	in percent, 0.01 % is realistic
			const float break_chance;

			const float speed;
			
			const float attack_rate;
			const float defense_rate;

			//	the average damage when fully sharp
			const float damage;

		protected :
			Weapon
			(
				string new_name,
				string mesh_name,
				float new_volume,
				float new_weight,
				float new_sharpness,
				float new_break_chance,
				float new_speed,
				float new_attack_rate,
				float new_defense_rate,
				float new_damage
			);
	};
}

#endif	//	TSL_WEAPON_HPP
