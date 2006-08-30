#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "inventory.hpp"

using namespace std;

class Character:
	public Obstacle
{
	public:
		Character (string new_name);
		virtual ~Character ();
		virtual bool is_initialized () const;
		float get_total_weight () const;

		//	exp = experience
		int agility_exp;
		int beauty_exp;
		int constitution_exp;
		int equolence_exp;
		int intelligence_exp;
		int quickness_exp;
		int strength_exp;
		int willpower_exp;

		Inventory * inventory;
};

#endif
