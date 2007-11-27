#ifndef RADAKAN_CHARACTER_HPP
#define RADAKAN_CHARACTER_HPP

#include "container_item.hpp"
#include "observable.hpp"
#include "observer.hpp"
#include "pointer.hpp"

using namespace std;

namespace Radakan
{
	template <typename T, class U> class Map;
	class Skill;

	namespace Messages
	{
		template <class T> class Message;
	}
	
	class Movable_Model;

	namespace Items
	{
		template <class T> class Container_Item;

		///	Character is the abstract bace class of all in-game characters.
		///	All characters observe each other.
		class Character :
			public Observable <Messages :: Message <Character> >,
			public Observer <Messages :: Message <Character> >,
			public Container_Item <Item>
		{
			private :
				static Reference <Set <Character> > characters;

			public :
				static string get_class_name ();

				static string Arachnyd;
				static string Grogg;
				static string Harpy;
				static string Inferno;
				static string Xemna;
			
				//	The constructor is protected, see below.
				virtual ~Character ();
				virtual bool is_initialized () const;
				
				virtual void call (const Reference <Messages :: Message <Character> > & message) = 0;
				
				virtual bool is_dead () const = 0;
				virtual void die () = 0;

				Reference <Movable_Model> get_movable_model () const;

				void hit (string fight_mode, Reference <Character> target);

				float get_skill (const string & skill_name) const;

				const string specie;
				
				//	Item & head;
				//	Multislot <Shirt> & body;
				Reference <Container_Item <Container_Item <Item> > > back;
				//	Multislot <Bracer> & arms;
				Reference <Container_Item <Item> > left_hand;
				Reference <Container_Item <Item> > right_hand;
				//	Multislot <Pants> & legs;
				//	Multislot <Shoe> & feet;

				mutable Reference <Map <string, Skill> > skills;

			protected :
				Character
				(
					string new_mesh_name,
					Ogre :: Vector3 new_volume,
					float new_mass
				);

			private :
				mutable Pointer <Movable_Model> movable_model;
		};
	}
}

#endif	//	RADAKAN_CHARACTER_HPP
