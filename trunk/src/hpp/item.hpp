#ifndef RADAKAN_ITEM_HPP
#define RADAKAN_ITEM_HPP

#include "resident.hpp"

using namespace std;

namespace Ogre
{
	class Entity;
}

namespace OgreOde
{
	class Geometry;
}

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
				virtual ~Item ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				virtual float get_total_mass () const;

				virtual boost :: shared_ptr <OgreOde :: Geometry> create_geometry ();

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
				
				///	false means that the object is static
				const bool mobile;
				
				///	false means that you can pass through it
				const bool solid;
				
				const bool visible;

				boost :: shared_ptr <Ogre :: Entity> entity;

			protected :
				Item
				(
					string new_mesh_name,
					Ogre :: Vector3 new_size,
					float new_mass,
					bool new_mobile = true,
					bool new_solid = true,
					bool new_visible = true
				);

			private :
				///	can be NULL
				Reference <Model> model;
		};
	}
}

#endif	//	RADAKAN_ITEM_HPP
