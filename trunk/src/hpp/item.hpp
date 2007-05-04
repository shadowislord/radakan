#ifndef TSL_ITEM_HPP
#define TSL_ITEM_HPP

#include "environment.hpp"

#include <OgreEntity.h>

using namespace std;

namespace tsl
{
	class Body;

	class Item :
		public virtual Object
	{
		public :
			virtual ~Item ();
			virtual bool is_initialized () const;
			static string get_class_name ();
			virtual float get_total_mass () const;

			virtual OgreOde :: Geometry & create_geometry ();

			///	Don't use this method directely, use 'new Body (...);' if possible.
			void set_body (Body & new_body);
			bool has_body () const;
			
			///	Don't use this method directely, use 'delete body;' if possible.
			void remove_body ();
			Body & get_body () const;

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
			Body * body;
	};
}

#endif	//	TSL_ITEM_HPP
