#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "container.hpp"

using namespace std;

class Character:
	public Container
{
	public:
		Character
			(string new_name,
			Ogre :: Entity * new_ogre_entity,
			Ogre :: SceneNode * new_node);
		virtual ~Character ();
		virtual bool is_initialized () const;
		virtual float get_total_weight () const;
		virtual bool add (Entity * item);
		virtual bool remove (Entity * item);
		virtual bool contains (Entity * item) const;
		bool has_weapon () const;
		bool is_dead () const;
		Weapon * get_weapon () const;
		void die ();
		
	private:
		//	exp = experience
		int agility_exp;
		int beauty_exp;
		int constitution_exp;
		int equolence_exp;
		int intelligence_exp;
		int quickness_exp;
		int strength_exp;
		int willpower_exp;
		
		bool dead;
		Weapon * weapon;
		Container * backpack;
};

#endif
