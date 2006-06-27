#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "inventory.hpp"

using namespace std;

class Character:
	public Obstacle
{
	public:
		Character (string new_name/*, Vector3 new_position, D3 new_size*/);
		virtual ~Character ();

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
