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
				static string get_class_name ();
				
				Static_Item
				(
					float new_mass,
					Mathematics :: Vector_3D new_size,
					const Reference <Mesh_Data> new_mesh_data
				);
				virtual ~Static_Item ();
				virtual bool is_initialized () const;

			private :

				static unsigned int counter;
		};
	}
}

#endif	//	RADAKAN_STATIC_ITEM_HPP
