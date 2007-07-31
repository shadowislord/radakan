#ifndef TSL_CHARACTER_HPP
#define TSL_CHARACTER_HPP

#include "multislot.hpp"
#include "observable.hpp"
#include "static_item.hpp"
#include "weapon.hpp"

#include <tinyxml.h>

using namespace std;

namespace TSL
{
	class Movable_Model;

	namespace Items
	{

		///	Character is the abstract bace class of all in-game characters.
		///	All characters observe each other.
		class Character :
			public Observable <Character>,
			public Observer <Character>,
			public Container
		{
			public :
				//	The constructor is protected, see below.
				virtual ~Character ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				virtual void call (const Object & message) = 0;
				
				virtual bool is_dead () const = 0;
				virtual void die () = 0;

				Movable_Model & get_movable_model () const;

				void chat (TiXmlElement & option, Character & target);
				void hit (string fight_mode, Character & target);
				
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
					Ogre :: Vector3 new_volume,
					float new_mass
				);

			private :
				int agility_exp;
				int beauty_exp;
				int constitution_exp;
				int equolence_exp;
				int intelligence_exp;
				int quickness_exp;
				int strength_exp;
				int willpower_exp;

				Movable_Model * movable_model;

				static Set <Character> characters;
		};
	}
}

#endif	//	TSL_CHARACTER_HPP
