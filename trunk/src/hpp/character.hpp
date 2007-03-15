#ifndef TSL_CHARACTER_HPP
#define TSL_CHARACTER_HPP

#include "multislot.hpp"
#include "static_item.hpp"
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
			
			virtual OgreOde :: Geometry & create_geometry ();
			
			virtual bool is_dead () const = 0;
			virtual void die () = 0;

			//	Item & head;
			//	Multislot <Shirt> & body;
			Multislot <Container> & back;
			//	Multislot <Bracer> & arms;
			Multislot <Item> & hands;
			//	Multislot <Pants> & legs;
			//	Multislot <Shoe> & feet;

		protected :
			Character
			(
				string new_mesh_name,
				float new_volume,
				float new_mass
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
