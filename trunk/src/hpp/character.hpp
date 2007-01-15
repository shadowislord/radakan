#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "container.hpp"

using namespace std;

namespace tsl
{

	///	Character contains all data of one in-game character.

	class Character :
		public Container
	{
		public :
			//	The constructor is protected, see below.
			virtual ~Character ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			virtual float get_total_weight () const;
			virtual bool add (Entity & entity);		//	true iff succes
			virtual bool move_to (Entity & entity, Set <Entity> & other_set);
			bool has_weapon () const;
			bool is_dead () const;
			Weapon * get_weapon () const;
			void walk (float distance);
			void turn (float radian_angle);
			virtual string die ();

		protected :
			Character
			(
				float new_volume,
				float new_weight,
				btVector3 new_position,
				Ogre :: SceneNode & new_node
			);

		private :
			//	Copies are not allowed.
			Character (const Character & character);
			
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
