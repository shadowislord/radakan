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

		int agility;
		int beauty;
		int constitution;
		int equolence;
		int intelligence;
		int quickness;
		int strength;
		int willpower;

		Inventory * inventory;
};

#endif
