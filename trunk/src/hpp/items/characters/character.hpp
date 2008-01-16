#ifndef RADAKAN_CHARACTERS_CHARACTER_HPP
#define RADAKAN_CHARACTERS_CHARACTER_HPP

#include "items/container_item.hpp"
#include "observable.hpp"
#include "observer.hpp"
#include "strategy_state_machine.hpp"
#include "strategies/behaviors/behavior.hpp"

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

		namespace Characters
		{
		
			///	All characters observe each other.
			class Character :
				public Observable <Messages :: Message <Character> >,
				public Observer <Messages :: Message <Character> >,
				public Container_Item <Item>,
				public Strategy_State_Machine
					<Strategies :: Behaviors :: Behavior, Character>
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
					
					virtual void call (const Reference <Messages :: Message <Character> > & message);
					
					bool is_dead () const;

					Reference <Movable_Model> get_movable_model () const;

					void hit (string fight_mode, Reference <Character> target);

					float get_skill (const string & skill_name) const;

					static Reference <Item> create_npc
					(
						string new_name,
						float new_mass,
						Ogre :: Vector3 new_size,
						const Reference <Mesh_Data> new_mesh_data
					);

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
						string new_name,
						float new_mass,
						Ogre :: Vector3 new_size,
						const Reference <Mesh_Data> new_mesh_data
					);

				private :
					mutable Pointer <Movable_Model> movable_model;
			};
		}
	}
}

#endif	//	RADAKAN_CHARACTERS_CHARACTER_HPP
