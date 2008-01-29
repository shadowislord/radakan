#ifndef RADAKAN_ITEMS_CHARACTER_HPP
#define RADAKAN_ITEMS_CHARACTER_HPP

#include "items/container_item.hpp"
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
		namespace Communications
		{
			class Communication;
		}
		
		class Nothing;
	}
	
	class Movable_Model;

	namespace Items
	{
		template <class T> class Container_Item;

		///	All characters observe each other.
		class Character :
			public Container_Item <Item>,
			public Observer <Messages :: Nothing>,
			public Strategy_State_Machine
				<Strategies :: Behaviors :: Behavior,
					Messages :: Nothing>
		{
			public :
				static string get_class_name ();

				static Reference <Character> get_player_character ();

				static string Arachnyd;
				static string Grogg;
				static string Harpy;
				static string Inferno;
				static string Xemna;

				Character
				(
					string new_name,
					float new_mass,
					Ogre :: Vector3 new_size,
					const Reference <Mesh_Data> new_mesh_data,
					string type	//	'npc' or 'player character'
				);
				virtual ~Character ();
				virtual bool is_initialized () const;

				///	'drop (...)' overrides the default.
				virtual void drop
					(Reference <Strategies :: Behaviors :: Behavior> behavior);

				virtual void call (Reference <Messages :: Nothing> message);

				bool is_alive () const;

				Reference <Movable_Model> get_movable_model () const;

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

			private :
				template <class T> void behave ();

				mutable Pointer <Movable_Model> movable_model;
		};
	}
}

#endif	// RADAKAN_ITEMS_CHARACTER_HPP
