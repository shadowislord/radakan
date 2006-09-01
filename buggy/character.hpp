#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "container.hpp"

using namespace std;

class Character:
	public Container
{
	public:
		Character (string new_name);
		virtual ~Character ();
		virtual bool is_initialized () const;
		virtual float get_total_weight () const;
		virtual bool add (Entity * item);
		virtual bool remove (Entity * item);
		virtual bool contains (Entity * item) const;

		bool dead;
		Weapon * weapon;
		
		//	exp = experience
		int agility_exp;
		int beauty_exp;
		int constitution_exp;
		int equolence_exp;
		int intelligence_exp;
		int quickness_exp;
		int strength_exp;
		int willpower_exp;

		Container * backpack;
};

#endif
