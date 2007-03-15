#ifndef TSL_CONTAINER_HPP
#define TSL_CONTAINER_HPP

#include "weapon.hpp"
#include "disjoint_set.hpp"

using namespace std;

namespace tsl
{

	///	A Container is an Item that can contain other Items.

	class Container :
		public Item,
		public Disjoint_Set <Item>
	{
		public :
			virtual ~Container ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			virtual float get_total_mass () const;

			///	'add' returns false if I'm full.
			virtual bool add (Item & item);

			virtual OgreOde :: Geometry & create_geometry ();
			
			static Item & create
			(
				string new_name,
				string mesh_name,
				float new_volume,
				float new_mass,
				bool new_mobile = true,
				bool new_solid = true,
				bool new_visible = true
			);

		protected :
			Container
			(
				string new_name,
				string new_mesh_name,
				float new_volume,
				float new_mass,
				bool new_mobile,
				bool new_solid,
				bool new_visible
			);

		private :
			///	Copies are not allowed.
			Container (const Container & container);
	};
}

#endif	//	TSL_CONTAINER_HPP
