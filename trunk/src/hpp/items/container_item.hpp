#ifndef RADAKAN_CONTAINER_ITEM_HPP
#define RADAKAN_CONTAINER_ITEM_HPP

#include "location.hpp"
#include "item.hpp"

using namespace std;

namespace Radakan
{
	namespace Items
	{

		template <class T> class Container_Item :
			public Resident <Container_Item <T> >,
			public Item,
			public Location <T>
		{
			public :
				static string get_class_name ();

				Container_Item
				(
					string new_name,
					float new_mass,
					Ogre :: Vector3 new_size,
					const Reference <Mesh_Data> new_mesh_data,
					int new_maximal_size = Container <T> :: unlimited()
				);
				virtual ~Container_Item ();
				virtual bool is_initialized () const;

				virtual float get_total_mass () const;
				
				///	On success, I return 'true'.
				virtual bool add (Reference <T> additive);
				
				virtual void drop (Reference <T> additive);

			private :
				float filled_volume;
				float total_mass;
		};
	}
}

#endif	//	RADAKAN_CONTAINER_ITEM_HPP
