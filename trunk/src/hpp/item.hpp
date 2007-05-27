#ifndef TSL_ITEM_HPP
#define TSL_ITEM_HPP

#include "object.hpp"

#include <OgreEntity.h>
#include <OgreOde_Core.h>

using namespace std;

namespace TSL
{
	class Model;
}

namespace TSL
{
	namespace Items
	{
		///	Item is the base class for all in-game items.
		class Item :
			public virtual Object
		{
			public :
				virtual ~Item ();
				virtual bool is_initialized () const;
				
				static const string get_class_name ();
				
				virtual float get_total_mass () const;

				virtual OgreOde :: Geometry & create_geometry ();

				///	Don't use this method directely, use 'new Body (...);' if possible.
				void set_model (Model & new_model);
				bool has_model () const;
				
				///	Don't use this method directely, use 'delete model;' if possible.
				void remove_model ();
				Model & get_model () const;

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

				Ogre :: Entity & entity;

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
				Model * model;
		};
	}
}

#endif	//	TSL_ITEM_HPP
