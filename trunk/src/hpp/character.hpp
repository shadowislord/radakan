#ifndef RADAKAN_CHARACTER_HPP
#define RADAKAN_CHARACTER_HPP

#include "container_item.hpp"
#include "observable.hpp"
#include "observer.hpp"

using namespace std;

namespace Radakan
{
	class Movable_Model;

	namespace Items
	{
		template <class T> class Container_Item;

		///	Character is the abstract bace class of all in-game characters.
		///	All characters observe each other.
		class Character :
			public Observable <Character>,
			public Observer <Character>,
			public Container_Item <Item>
		{
			public :
				//	The constructor is protected, see below.
				virtual ~Character ();
				virtual bool is_initialized () const;
				
				static string get_class_name ();

				virtual void call (const Reference <Object> message = update) = 0;
				
				virtual bool is_dead () const = 0;
				virtual void die () = 0;

				Reference <Movable_Model> get_movable_model () const;

				void hit (string fight_mode, Reference <Character> target);

				float get_skill (const string & skill_name) const;
				void add_experience (const string & skill_name, float amount = 1);
				
				//	Item & head;
				//	Multislot <Shirt> & body;
				Reference <Container_Item <Container_Item <Item> > > back;
				//	Multislot <Bracer> & arms;
				Reference <Container_Item <Item> > left_hand;
				Reference <Container_Item <Item> > right_hand;
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
				static const float default_experience;

				mutable Reference <Movable_Model> movable_model;

				static Reference <Set <Character> > characters;

				map <const string, float> experiences;
				
		};
	}
}

#endif	//	RADAKAN_CHARACTER_HPP
