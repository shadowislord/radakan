#ifndef TSL_STATIC_ITEM_HPP
#define TSL_STATIC_ITEM_HPP

#include "item.hpp"

using namespace std;

namespace TSL
{
	namespace Items
	{

		///	Static_Item is an Item that can't move.
		class Static_Item :
			public Item
		{
			public :
				virtual ~Static_Item ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();

				static Item & create
				(
					string new_name,
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass,
					bool new_solid = true,
					bool new_visible = true
				);

			protected :
				Static_Item
				(
					string new_name,
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass,
					bool new_solid = true,
					bool new_visible = true
				);
		};
	}
}

#endif	//	TSL_STATIC_ITEM_HPP
