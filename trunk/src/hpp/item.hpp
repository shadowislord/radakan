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

			virtual OgreOde :: Geometry & create_geometry () = 0;

			///	Don't use this method, use 'new Body (...);' instead.
			void set_body (Body & new_body);
			bool has_body () const;
			
			///	Don't use this method, use 'delete body;' instead.
			void remove_body ();
			Body & get_body () const;

			///	in litres
			const float volume;
			
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
				float new_volume,
				float new_mass,
				bool new_mobile,
				bool new_solid,
				bool new_visible
			);

		private :
			//	Copies are not allowed.
			Item (const Item & item);

			///	can be NULL
			Body * body;
	};
}

#endif	//	TSL_ITEM_HPP
