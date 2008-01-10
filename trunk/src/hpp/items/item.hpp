#ifndef RADAKAN_ITEM_HPP
#define RADAKAN_ITEM_HPP

#include "mesh_data.hpp"
#include "resident.hpp"

using namespace std;

namespace Radakan
{
	class Model;

	namespace Items
	{
		///	Item is the base class for all in-game items.
		class Item :
			public Resident <Item>
		{
			public :
				static string get_class_name ();
				
				virtual ~Item ();
				virtual bool is_initialized () const;
				
				virtual float get_total_mass () const;

				///	Don't use this method directely.
				void set_model (Reference <Model> new_model);
				
				bool has_model () const;
				
				///	Don't use this method directely, use 'model . destruct ();' if possible.
				void remove_model ();
				
				Reference <Model> get_model () const;

				float get_volume () const;

				///	in cubic meters
				const Ogre :: Vector3 size;
				
				///	in kilograms
				const float mass;
				
				Reference <Mesh_Data> mesh_data;

			protected :
				Item
				(
					float new_mass,
					Ogre :: Vector3 new_size,
					const Reference <Mesh_Data> new_mesh_data
				);

			private :
				Pointer <Model> model;
		};
	}
}

#endif	//	RADAKAN_ITEM_HPP
