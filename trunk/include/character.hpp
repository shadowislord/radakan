#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "container.hpp"

using namespace std;

namespace sl
{

	///	Character contains all data of one in-game character.

	///
	///	It is called by an engine to do something, or it's data is retrieved by Ogre.
	class Character :
		public Container
	{
		public :
			Character
				(Ogre :: SceneNode * new_node);
			virtual ~Character ();
			virtual bool is_initialized () const;
			virtual float get_total_weight () const;
			virtual bool add (Entity * entity);		//	true iff succes
			virtual bool move_to (Entity * entity, Set <Entity> * other_set);
			bool has_weapon () const;
			bool is_dead () const;
			Weapon * get_weapon () const;
			void run (float distance);
			void turn (float radian_angle);
			string die ();
		
		private :
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
}

#endif
