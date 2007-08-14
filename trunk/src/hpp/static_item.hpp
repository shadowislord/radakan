#ifndef RADAKAN_STATIC_ITEM_HPP
#define RADAKAN_STATIC_ITEM_HPP

#include "item.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{

		///	Static_Item is an Item that can't move.
		class Static_Item :
			public Item
		{
			public :
				Static_Item
				(
					string new_name,
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass,
					bool new_solid = true,
					bool new_visible = true
				);
				virtual ~Static_Item ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
		};
	}
}

#endif	//	RADAKAN_STATIC_ITEM_HPP
