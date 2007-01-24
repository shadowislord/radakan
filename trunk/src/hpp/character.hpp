#ifndef TSL_CHARACTER_HPP
#define TSL_CHARACTER_HPP

#include "multislot.hpp"
#include "weapon.hpp"

using namespace std;

namespace tsl
{

	///	Character is the abstract bace class of all in-game characters.

	class Character :
		public Container
	{
		public :
			//	The constructor is protected, see below.
			virtual ~Character ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			
			virtual bool is_dead () const = 0;
			virtual string die () = 0;

			//	Multislot <Hat> & head;
			//	Multislot <Shirt> & body;
			Multislot <Container> & back;
			//	Multislot <Bracer> & arms;
			Multislot <Item> & hands;
			//	Multislot <Pants> & legs;
			//	Multislot <Shoe> & feet;

		protected :
			Character
			(
				string mesh_name,
				float new_volume,
				float new_weight
			);

		private :
			///	Copies are not allowed.
			Character (const Character & character);
			
			int agility_exp;
			int beauty_exp;
			int constitution_exp;
			int equolence_exp;
			int intelligence_exp;
			int quickness_exp;
			int strength_exp;
			int willpower_exp;
	};
}

#endif	//	TSL_CHARACTER_HPP
