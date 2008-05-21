#ifndef RADAKAN_WEAPON_HPP
#define RADAKAN_WEAPON_HPP

#include "item.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{

		class Weapon :
			public Item
		{
			public :
				Weapon
				(
					string new_name,
					float new_mass,
					Mathematics :: Vector_3D new_size,
					const Reference <Mesh_Data> new_mesh_data,
					float new_sharpness,
					float new_break_chance,
					float new_speed,
					float new_attack_rate,
					float new_defense_rate,
					float new_damage
				);
				virtual ~Weapon ();
				virtual bool is_initialized () const;
				
				static string get_class_name ();

				bool broken;
				
				///	between 0 and 1
				float sharpness;

				///	between 0 and 1 (0.001 is realistic)
				const float break_chance;

				const float speed;
				
				const float attack_rate;
				const float defense_rate;

				//	the average damage when fully sharp
				const float damage;

			protected :
		};
	}
}

#endif	//	RADAKAN_WEAPON_HPP