#ifndef RADAKAN_CONTAINER_HPP
#define RADAKAN_CONTAINER_HPP

#include "weapon.hpp"
#include "set.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{

		///	A Container is an Item that can contain other Items.
		class Container :
			public Item,
			public Location <Item>
		{
			public :
				Container
				(
					string new_name,
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass,
					bool new_mobile = true,
					bool new_solid = true,
					bool new_visible = true,
					int new_maximal_size = Set <Item> :: unlimited
				);
				virtual ~Container ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				///	'add' returns false if I'm full.
				virtual bool add (Item & item);

				virtual float get_total_mass () const;
		};
	}
}

#endif	//	RADAKAN_CONTAINER_HPP
