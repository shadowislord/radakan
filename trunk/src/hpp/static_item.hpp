#ifndef TSL_STATIC_ITEM_HPP
#define TSL_STATIC_ITEM_HPP

#include "item.hpp"

using namespace std;

namespace tsl
{
	class Static_Item :
	public Item
	{
		public :
			virtual ~Static_Item ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			static Item & create
			(
				string new_name,
				string new_mesh_name,
				float new_volume,
				float new_mass
			);

		protected :
			Static_Item
			(
				string new_name,
				string new_mesh_name,
				float new_volume,
				float new_mass
			);
	};
}

#endif	//	TSL_STATIC_ITEM_HPP
