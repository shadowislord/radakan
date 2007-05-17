#ifndef TSL_CONTAINER_HPP
#define TSL_CONTAINER_HPP

#include "weapon.hpp"
#include "set.hpp"
#include "movable_model.hpp"

using namespace std;

namespace TSL
{

	///	A Container is an Item that can contain other Items.
	class Container :
		public Item,
		public Set <Item>
	{
		public :
			virtual ~Container ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();
			
			///	'add' returns false if I'm full.
			virtual bool add (Item & item);

			virtual float get_total_mass () const;

			static Item & create
			(
				string new_name,
				string mesh_name,
				Ogre :: Vector3 new_size,
				float new_mass,
				bool new_mobile = true,
				bool new_solid = true,
				bool new_visible = true,
				int new_maximal_size = Set <Item> :: unlimited
			);

		protected :
			Container
			(
				string new_name,
				string new_mesh_name,
				Ogre :: Vector3 new_size,
				float new_mass,
				bool new_mobile,
				bool new_solid,
				bool new_visible,
				int new_maximal_size
			);
	};
}

#endif	//	TSL_CONTAINER_HPP
